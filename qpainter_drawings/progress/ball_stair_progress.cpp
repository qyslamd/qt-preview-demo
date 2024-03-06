#include "ball_stair_progress.h"

#include <QGraphicsProxyWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QSignalTransition>
#include <QState>
#include <QStateMachine>
#include <QStyleOptionGraphicsItem>
#include <QTimer>
#include <QVariantAnimation>
#include <QtDebug>

using namespace ball_stair_progress;

AnimationPoint::AnimationPoint(const QRectF &rect, QObject *parent)
    : QObject(parent), rect_(rect) {
  statem_ = new QStateMachine(this);
  timer_ = new QTimer(this);
  connect(statem_, &QStateMachine::started, [this] { timer_->start(1000); });
  connect(statem_, &QStateMachine::stopped, [this] { timer_->stop(); });

  for (int i = 0; i < Size; i++) {
    array_state_[i] = new QState;
    statem_->addState(array_state_[i]);
  }

  connect(array_state_[5], &QState::entered,
          [this] { emit revertChanged(false); });
  connect(array_state_[9], &QState::exited,
          [this] { emit revertChanged(true); });

  setupStateMachine();
}

AnimationPoint::~AnimationPoint() { statem_->stop(); }

QPointF AnimationPoint::pos() const { return pos_; }

void AnimationPoint::setPos(const QPointF &pos) { pos_ = pos; }

qreal AnimationPoint::minHLen() const { return min_h_len_; }

qreal AnimationPoint::minWLen() const { return min_w_len_; }

qreal AnimationPoint::spacing() const { return spacing_; }

void AnimationPoint::setupStateMachine() {
  if (statem_->isRunning()) {
    statem_->stop();
  }

  for (int i = 0; i < Size; i++) {
    clearStateTransitions(array_state_[i]);
  }

  // 高度最小单位
  auto minWH = std::min(rect_.width(), rect_.height());
  auto hh = minWH / 6.0;
  min_h_len_ = hh;

  // 宽度最小单位
  auto ww = minWH / 7.0;
  min_w_len_ = ww;
  spacing_ = ww / 4.0;

  for (int i = 0; i < Size; i++) {
    if (i < 5) {
      array_pos_[i] =
          QPointF(ww * (i + 1) + i * spacing_, minWH - hh * (i + 1));
    } else {
      array_pos_[i] = QPointF(ww * (Size - i) + (Size - 1 - i) * spacing_,
                              minWH - hh * (i - 4));
    }
    array_state_[i]->assignProperty(this, "pos", array_pos_[i]);
  }

  for (int i = 0; i < Size; i++) {
    if (i == Size - 1) {
      auto trans = array_state_[i]->addTransition(timer_, &QTimer::timeout,
                                                  array_state_[0]);
      trans->addAnimation(createVariantAnimation(array_pos_[i], array_pos_[0]));
    } else {
      auto trans = array_state_[i]->addTransition(timer_, &QTimer::timeout,
                                                  array_state_[i + 1]);
      trans->addAnimation(
          createVariantAnimation(array_pos_[i], array_pos_[i + 1]));
    }
  }

  statem_->setInitialState(array_state_[0]);
  statem_->start();
}

void AnimationPoint::clearStateTransitions(QState *state) {
  if (!state) return;
  auto transitions = state->transitions();
  for (auto item : transitions) {
    clearTransitionAnime(item);
    state->removeTransition(item);
  }
}

void AnimationPoint::clearTransitionAnime(QAbstractTransition *trans) {
  if (!trans) return;
  auto animations = trans->animations();
  for (auto item : animations) {
    trans->removeAnimation(item);
    item->deleteLater();
  }
}

QAbstractAnimation *AnimationPoint::createVariantAnimation(
    const QVariant &start, const QVariant &end) {
  auto anime = new QVariantAnimation(this);
  connect(anime, &QVariantAnimation::valueChanged,
          [this](const QVariant &value) {
            setPos(value.value<QPointF>());
            emit posChanged(pos_);
          });
  anime->setStartValue(start);

  auto midPos = start.value<QPointF>();
  midPos.ry() = end.value<QPointF>().y();
  anime->setKeyValueAt(0.5, midPos);

  anime->setEndValue(end);
  anime->setDuration(800);
  return anime;
}

BallStairWidget::BallStairWidget(QWidget *parent) : QWidget(parent) {
  setAttribute(Qt::WA_TranslucentBackground);
}

void BallStairWidget::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);
  QPainter p(this);
  drawProgress(&p);
}

void BallStairWidget::drawProgress(QPainter *p) {
  if (!point_) {
    point_ = new AnimationPoint(this->rect(), this);
    connect(point_, &AnimationPoint::posChanged,
            [this](const QPointF &) { update(); });
    connect(point_, &AnimationPoint::revertChanged, [=](bool revert) {
      need_revert_ = revert;
      update();
    });
  }
  auto penWidth = point_->minHLen();

  p->save();
  p->setRenderHint(QPainter::Antialiasing);
  p->setPen(QPen(QBrush(QColor(0xFE8939)), penWidth, Qt::SolidLine,
                 Qt::RoundCap, Qt::RoundJoin));
  p->setBrush(Qt::NoBrush);

  p->drawPoint(point_->pos());
  p->restore();

  auto minWH = std::min(width(), height());
  auto hh = point_->minHLen();

  // 宽度最小单位
  auto ww = point_->minWLen();
  auto spacing = point_->spacing();

  QVector<QLineF> lines;
  for (int i = 0; i < 5; i++) {
    auto x = ww * (i + 1) + i * spacing;
    if (need_revert_) {
      lines << QLineF(x, minWH - hh * i, x, minWH);
    } else {
      lines << QLineF(x, minWH - hh * (4 - i), x, minWH);
    }
  }
  p->save();
  p->setRenderHint(QPainter::Antialiasing);
  p->setPen(QPen(QBrush(QColor(0xFE8939)), penWidth));
  p->setBrush(Qt::NoBrush);
  p->drawLines(lines);
  p->restore();
}
