#include "four_fish_progress.h"

#include <QGraphicsProxyWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionGraphicsItem>
#include <QTimer>
#include <QtDebug>
#include <QtMath>

FourFishRotateProgress::FourFishRotateProgress(QWidget *parent)
    : QWidget(parent) {
  setAttribute(Qt::WA_TranslucentBackground);
  auto timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, [this] { update(); });
  timer->start(25);
}

void FourFishRotateProgress::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);
  QPainter p(this);
  drawProgress(&p);
}

void FourFishRotateProgress::drawProgress(QPainter *p) {
  static auto count = 40;

  p->setPen(Qt::NoPen);
  p->setRenderHint(QPainter::Antialiasing);
  p->translate(this->rect().center());

  auto angle = tick_ / 8.0;
  // y=Asin(ωx+φ)+k
  // A:振幅
  // (ωx+φ):相位，反映y处的状态
  // k:反映在坐标系上图像的上移或下移
  auto radius = A * std::sin(tick_ / 15.0) + k;
  auto size = 0.0;
  auto halfSize = 0.0;
  QRectF rect;

  for (int i = 0; i < count; i++) {
    angle += M_PI / 64.0;
    radius += i / 30.0;
    size = size_base_ + i / size_div_;
    halfSize = size / 2.0;

    rect.setWidth(size);
    rect.setHeight(size);
    rect.moveCenter(
        QPointF(std::cos(angle) * radius, std::sin(angle) * radius));
    p->setBrush(color1);
    p->drawRoundedRect(rect, halfSize, halfSize);

    rect.setWidth(size);
    rect.setHeight(size);
    rect.moveCenter(
        QPointF(std::cos(angle) * -radius, std::sin(angle) * -radius));
    p->setBrush(color2);
    p->drawRoundedRect(rect, halfSize, halfSize);

    rect.setWidth(size);
    rect.setHeight(size);
    rect.moveCenter(QPointF(std::cos(angle + M_PI_2) * radius,
                            std::sin(angle + M_PI_2) * radius));
    p->setBrush(color3);
    p->drawRoundedRect(rect, halfSize, halfSize);

    rect.setWidth(size);
    rect.setHeight(size);
    rect.moveCenter(QPointF(std::cos(angle + M_PI_2) * -radius,
                            std::sin(angle + M_PI_2) * -radius));
    p->setBrush(color4);
    p->drawRoundedRect(rect, halfSize, halfSize);
  }
  tick_++;
}
