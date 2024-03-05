#include "hourglass.h"

#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QTimer>
#include <QtDebug>
#include <cmath>

static bool doubleEquals(const double lhs, const double rhs) {
  return abs(lhs - rhs) <= 1e-6;
}

HourglassWidget::HourglassWidget(QWidget* parent) : QWidget(parent) {
  timer_ = new QTimer(this);
  connect(timer_, &QTimer::timeout, this, &HourglassWidget::UpdateAngle);
  timer_->start(rotate_speed_);
}

void HourglassWidget::paintEvent(QPaintEvent* event) {
  QPainter painter(this);

  drawSandClock(&painter);

  Q_UNUSED(event)
}

void HourglassWidget::drawSandClock(QPainter* painter) {
  painter->setRenderHints(QPainter::Antialiasing |
                          QPainter::HighQualityAntialiasing);

  auto radiusOut = std::min(width(), height()) / 2.0;

  if (now_status_ == DRAW_CIR_ROTATE) {
    triangleH = radiusOut;
  }
  painter->save();

  // move to center
  painter->translate(rect().center());
  painter->setPen(QPen(frame_color_, frame_width_, Qt::SolidLine, Qt::RoundCap,
                       Qt::RoundJoin));

  painter->rotate(rotate_angle_);

  QPainterPath sandClockPath;
  sandClockPath.moveTo(-radiusOut * 0.05, radiusOut * 0.05);
  sandClockPath.lineTo(radiusOut * 0.1, radiusOut);
  sandClockPath.lineTo(radiusOut, radiusOut * 0.1);
  sandClockPath.lineTo(radiusOut * 0.05, -radiusOut * 0.05);
  sandClockPath.lineTo(-radiusOut * 0.1, -radiusOut);
  sandClockPath.lineTo(-radiusOut, -radiusOut * 0.1);
  sandClockPath.lineTo(-radiusOut * 0.05, radiusOut * 0.05);
  painter->drawPath(sandClockPath);

  painter->setPen(Qt::NoPen);
  QPointF triangle[3];
  painter->setBrush(QBrush(sand_color_, Qt::SolidPattern));  //设置画刷形式
  if (now_status_ == State_UpTriangle) {
    if (is_drawing_triangle_) {
      triangle[0] = QPointF(0, 0);
      triangle[1] = QPointF(-triangleH * 0.1, -triangleH);
      triangle[2] = QPointF(-triangleH, -triangleH * 0.1);
      triangleH -= 1;
      painter->drawPolygon(triangle, 3);
      triangle[0] = QPointF(0, 0);
      triangle[1] =
          QPointF((radiusOut * 0.1) - triangleH * 0.1, radiusOut - triangleH);
      triangle[2] =
          QPointF(radiusOut - triangleH, (radiusOut * 0.1) - triangleH * 0.1);
      if (triangleH < 0) {
        is_drawing_triangle_ = false;
        triangleH = radiusOut;
        now_status_ = DRAW_CIR_ROTATE;
        rotate_angle_ += 1;
      }
      painter->drawPolygon(triangle, 3);
    }
  } else if (now_status_ == State_DownTriangle) {
    if (is_drawing_triangle_) {
      triangle[0] = QPointF(0, 0);
      triangle[1] = QPointF(triangleH * 0.1, triangleH);
      triangle[2] = QPointF(triangleH, triangleH * 0.1);
      triangleH -= 1;
      painter->drawPolygon(triangle, 3);
      triangle[0] = QPointF(0, 0);
      triangle[1] =
          QPointF(-(radiusOut * 0.1) + triangleH * 0.1, -radiusOut + triangleH);
      triangle[2] =
          QPointF(-radiusOut + triangleH, -(radiusOut * 0.1) + triangleH * 0.1);

      if (triangleH <= 0) {
        is_drawing_triangle_ = false;
        triangleH = radiusOut;
        now_status_ = DRAW_CIR_ROTATE;
        rotate_angle_ += 1;
      }
      painter->drawPolygon(triangle, 3);
    }
  } else {
    if (rotate_angle_ > 45 && rotate_angle_ <= 225) {
      triangle[0] = QPointF(0, 0);
      triangle[1] = QPointF(radiusOut * 0.1 - frame_width_ / 3,
                            radiusOut - frame_width_ / 3);
      triangle[2] = QPointF(radiusOut - frame_width_ / 3,
                            radiusOut * 0.1 - frame_width_ / 3);
    } else {
      triangle[0] = QPointF(0, 0);
      triangle[1] = QPointF(-radiusOut * 0.1 + frame_width_ / 3,
                            -radiusOut + frame_width_ / 3);
      triangle[2] = QPointF(-radiusOut + frame_width_ / 3,
                            -radiusOut * 0.1 + frame_width_ / 3);
    }
    painter->drawPolygon(triangle, 3);
  }

  painter->restore();
}

void HourglassWidget::UpdateAngle() {
  if (doubleEquals(rotate_angle_, 45.0)) {
    now_status_ = State_UpTriangle;
    timer_->start(sand_drop_speed_);
    is_drawing_triangle_ = true;
  } else if (doubleEquals(rotate_angle_, 225.0)) {
    now_status_ = State_DownTriangle;
    timer_->start(sand_drop_speed_);
    is_drawing_triangle_ = true;
  } else {
    now_status_ = DRAW_CIR_ROTATE;
    timer_->setInterval(rotate_speed_);
  }
  if (!is_drawing_triangle_) {
    rotate_angle_ += 1;
    if (rotate_angle_ > 360) {
      rotate_angle_ = 0.0;
    }
  }
  update();
}
