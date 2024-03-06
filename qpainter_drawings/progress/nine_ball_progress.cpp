#include "nine_ball_progress.h"

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
#include <QtMath>
#include <cmath>

bool AnimationPoint::Clockwise = false;
AnimationPoint::AnimationPoint(Gua gua, const QPointF &start_pos,
                               qreal square_width, QObject *obj)
    : QObject(obj),
      gua_(gua),
      mid_point_(start_pos),
      square_width_(square_width) {
  timer_ = new QTimer(this);
  statem_ = new QStateMachine(this);
  connect(statem_, &QStateMachine::started, this,
          [this]() { timer_->start(1000); });
  connect(statem_, &QStateMachine::stopped, this, [this]() { timer_->stop(); });

  state_li_ = new QState();
  state_xun_ = new QState();
  state_zhen_ = new QState();
  state_gen_ = new QState();
  state_kan_ = new QState();
  state_qian_ = new QState();
  state_dui_ = new QState();
  state_kun_ = new QState();

  statem_->addState(state_li_);
  statem_->addState(state_xun_);
  statem_->addState(state_zhen_);
  statem_->addState(state_gen_);
  statem_->addState(state_kan_);
  statem_->addState(state_qian_);
  statem_->addState(state_dui_);
  statem_->addState(state_kun_);

  setupStateMachine();
}

bool AnimationPoint::clockwise() { return Clockwise; }

QPointF AnimationPoint::pos() const { return pos_; }

void AnimationPoint::setPos(const QPointF &pos) { pos_ = pos; }

void AnimationPoint::setMidPoint(const QPoint &pos) {
  mid_point_ = pos;
  setupStateMachine();
}

void AnimationPoint::setSquareWidth(const int width) {
  square_width_ = width;
  setupStateMachine();
}

void AnimationPoint::setupStateMachine() {
  if (statem_->isRunning()) {
    statem_->stop();
  }

  clearStateTransitions(state_li_);
  clearStateTransitions(state_xun_);
  clearStateTransitions(state_zhen_);
  clearStateTransitions(state_gen_);
  clearStateTransitions(state_kan_);
  clearStateTransitions(state_qian_);
  clearStateTransitions(state_dui_);
  clearStateTransitions(state_kun_);

  // 勾股定理
  /*
   *  |\
   *  | \
   *  ----
   * */
  auto len = sqrt(square_width_ * square_width_ / 2.0);
  auto posLi = QPointF(mid_point_.x(), mid_point_.y() - len);
  auto posXun = QPointF(mid_point_.x() - len / 2.0, mid_point_.y() - len / 2.0);
  auto posZhen = QPointF(mid_point_.x() - len, mid_point_.y());
  auto posGen = QPointF(mid_point_.x() - len / 2.0, mid_point_.y() + len / 2.0);
  auto posKan = QPointF(mid_point_.x(), mid_point_.y() + len);
  auto posQian =
      QPointF(mid_point_.x() + len / 2.0, mid_point_.y() + len / 2.0);
  auto posDui = QPointF(mid_point_.x() + len, mid_point_.y());
  auto posKun = QPointF(mid_point_.x() + len / 2.0, mid_point_.y() - len / 2.0);

  state_li_->assignProperty(this, "pos", posLi);
  state_xun_->assignProperty(this, "pos", posXun);
  state_zhen_->assignProperty(this, "pos", posZhen);
  state_gen_->assignProperty(this, "pos", posGen);
  state_kan_->assignProperty(this, "pos", posKan);
  state_qian_->assignProperty(this, "pos", posQian);
  state_dui_->assignProperty(this, "pos", posDui);
  state_kun_->assignProperty(this, "pos", posKun);

  auto getVariantAnimation = [this](
                                 const QVariant &start,
                                 const QVariant &end) -> QAbstractAnimation * {
    auto anime = new QVariantAnimation(this);
    connect(anime, &QVariantAnimation::valueChanged, this,
            [this](const QVariant &value) {
              setPos(value.value<QPointF>());
              emit posChanged(pos_);
            });
    anime->setKeyValueAt(0, start);
    anime->setKeyValueAt(1, end);
    anime->setDuration(500);
    return anime;
  };

  if (Clockwise) {
    auto trans = state_li_->addTransition(timer_, &QTimer::timeout, state_kun_);
    trans->addAnimation(getVariantAnimation(posLi, posKun));

    trans = state_kun_->addTransition(timer_, &QTimer::timeout, state_dui_);
    trans->addAnimation(getVariantAnimation(posKun, posDui));

    trans = state_dui_->addTransition(timer_, &QTimer::timeout, state_qian_);
    trans->addAnimation(getVariantAnimation(posDui, posQian));

    trans = state_qian_->addTransition(timer_, &QTimer::timeout, state_kan_);
    trans->addAnimation(getVariantAnimation(posQian, posKan));

    trans = state_kan_->addTransition(timer_, &QTimer::timeout, state_gen_);
    trans->addAnimation(getVariantAnimation(posKan, posGen));

    trans = state_gen_->addTransition(timer_, &QTimer::timeout, state_zhen_);
    trans->addAnimation(getVariantAnimation(posGen, posZhen));

    trans = state_zhen_->addTransition(timer_, &QTimer::timeout, state_xun_);
    trans->addAnimation(getVariantAnimation(posZhen, posXun));

    trans = state_xun_->addTransition(timer_, &QTimer::timeout, state_li_);
    trans->addAnimation(getVariantAnimation(posXun, posLi));
  } else {
    auto trans = state_li_->addTransition(timer_, &QTimer::timeout, state_xun_);
    trans->addAnimation(getVariantAnimation(posLi, posXun));

    trans = state_xun_->addTransition(timer_, &QTimer::timeout, state_zhen_);
    trans->addAnimation(getVariantAnimation(posXun, posZhen));

    trans = state_zhen_->addTransition(timer_, &QTimer::timeout, state_gen_);
    trans->addAnimation(getVariantAnimation(posZhen, posGen));

    trans = state_gen_->addTransition(timer_, &QTimer::timeout, state_kan_);
    trans->addAnimation(getVariantAnimation(posGen, posKan));

    trans = state_kan_->addTransition(timer_, &QTimer::timeout, state_qian_);
    trans->addAnimation(getVariantAnimation(posKan, posQian));

    trans = state_qian_->addTransition(timer_, &QTimer::timeout, state_dui_);
    trans->addAnimation(getVariantAnimation(posQian, posDui));

    trans = state_dui_->addTransition(timer_, &QTimer::timeout, state_kun_);
    trans->addAnimation(getVariantAnimation(posDui, posKun));

    trans = state_kun_->addTransition(timer_, &QTimer::timeout, state_li_);
    trans->addAnimation(getVariantAnimation(posKun, posLi));
  }

  switch (gua_) {
    case Gua::Li:
      statem_->setInitialState(state_li_);
      break;
    case Xun:
      statem_->setInitialState(state_xun_);
      break;
    case Zhen:
      statem_->setInitialState(state_zhen_);
      break;
    case Gen:
      statem_->setInitialState(state_gen_);
      break;
    case Kan:
      statem_->setInitialState(state_kan_);
      break;
    case Qian:
      statem_->setInitialState(state_qian_);
      break;
    case Dui:
      statem_->setInitialState(state_dui_);
      break;
    case Kun:
      statem_->setInitialState(state_kun_);
      break;
  }
  statem_->start();
}

void AnimationPoint::clearStateTransitions(QState *state) {
  if (!state) return;
  auto transitions = state->transitions();
  foreach (auto item, transitions) {
    clearTransitionAnime(item);
    state->removeTransition(item);
  }
}

void AnimationPoint::clearTransitionAnime(QAbstractTransition *trans) {
  if (!trans) return;
  auto animations = trans->animations();
  foreach (auto item, animations) {
    trans->removeAnimation(item);
    item->deleteLater();
  }
}

NineBallProgress::NineBallProgress(QWidget *parent) : QWidget(parent) {
  setAttribute(Qt::WA_TranslucentBackground);
}

void NineBallProgress::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);
  QPainter p(this);
  drawer1(&p);
}

void NineBallProgress::drawer1(QPainter *p) {
  p->save();
  p->fillRect(rect(), QColor(0xBB564F));
  p->restore();

  qreal side_len_ = std::min(width(), height());
  side_len_ = side_len_ / M_SQRT2;
  auto penWidth = side_len_ / 4.0;
  side_len_ -= penWidth;

  p->save();
  p->setRenderHint(QPainter::Antialiasing);
  p->setPen(QPen(QBrush(QColor(0xF5D4A1)), side_len_ / 4.0, Qt::SolidLine,
                 Qt::RoundCap, Qt::RoundJoin));
  p->setBrush(Qt::NoBrush);
  auto midX = width() / 2.0;
  auto midY = height() / 2.0;

  //  auto pen = p->pen();
  //  pen.setBrush(QColor(0xE30405));
  //  p->setPen(pen);
  p->drawPoint(this->rect().center());

  if (!d1_) {
    d1_ = new AnimationPoint(AnimationPoint::Li, QPointF(midX, midY), side_len_,
                             this);
    connect(d1_, &AnimationPoint::posChanged, this,
            [this](const QPointF &) { update(); });
  }
  //  pen.setBrush(QColor(0xFD8C02));
  //  p->setPen(pen);
  p->drawPoint(d1_->pos());

  if (!d2_) {
    d2_ = new AnimationPoint(AnimationPoint::Kun, QPointF(midX, midY),
                             side_len_, this);
    connect(d2_, &AnimationPoint::posChanged, this,
            [this](const QPointF &) { update(); });
  }
  //  pen.setBrush(QColor(0xFFEE02));
  //  p->setPen(pen);
  p->drawPoint(d2_->pos());

  if (!d3_) {
    d3_ = new AnimationPoint(AnimationPoint::Dui, QPointF(midX, midY),
                             side_len_, this);
    connect(d3_, &AnimationPoint::posChanged, this,
            [this](const QPointF &) { update(); });
  }
  //  pen.setBrush(QColor(0x007F28));
  //  p->setPen(pen);
  p->drawPoint(d3_->pos());

  if (!d4_) {
    d4_ = new AnimationPoint(AnimationPoint::Qian, QPointF(midX, midY),
                             side_len_, this);
    connect(d4_, &AnimationPoint::posChanged, this,
            [this](const QPointF &) { update(); });
  }
  //  pen.setBrush(QColor(0x044BFB));
  //  p->setPen(pen);
  p->drawPoint(d4_->pos());

  if (!d5_) {
    d5_ = new AnimationPoint(AnimationPoint::Kan, QPointF(midX, midY),
                             side_len_, this);
    connect(d5_, &AnimationPoint::posChanged, this,
            [this](const QPointF &) { update(); });
  }
  //  pen.setBrush(QColor(0x740588));
  //  p->setPen(pen);
  p->drawPoint(d5_->pos());

  if (!d6_) {
    d6_ = new AnimationPoint(AnimationPoint::Gen, QPointF(midX, midY),
                             side_len_, this);
    connect(d6_, &AnimationPoint::posChanged, this,
            [this](const QPointF &) { update(); });
  }
  //  pen.setBrush(QColor(0x7FFF00));
  //  p->setPen(pen);
  p->drawPoint(d6_->pos());

  if (!d7_) {
    d7_ = new AnimationPoint(AnimationPoint::Zhen, QPointF(midX, midY),
                             side_len_, this);
    connect(d7_, &AnimationPoint::posChanged, this,
            [this](const QPointF &) { update(); });
  }
  //  pen.setBrush(QColor(0x00BFFF));
  //  p->setPen(pen);
  p->drawPoint(d7_->pos());

  if (!d8_) {
    d8_ = new AnimationPoint(AnimationPoint::Xun, QPointF(midX, midY),
                             side_len_, this);
    connect(d8_, &AnimationPoint::posChanged, this,
            [this](const QPointF &) { update(); });
  }
  //  pen.setBrush(QColor(0x9400D3));
  //  p->setPen(pen);
  p->drawPoint(d8_->pos());

  p->restore();
}
