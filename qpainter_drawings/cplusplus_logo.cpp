#include "cplusplus_logo.h"

#include <QFontMetricsF>
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QVariantAnimation>
#include <QtMath>

CplusplusWidget::CplusplusWidget(QWidget *parent) : QWidget(parent) {
  setAttribute(Qt::WA_TranslucentBackground);
  auto anime = new QVariantAnimation(this);
  anime->setStartValue(0.0);
  anime->setEndValue(360.0);
  anime->setDuration(3000);
  anime->setLoopCount(-1);
  //    anime->setEasingCurve(QEasingCurve::OutInQuad);
  connect(anime, &QVariantAnimation::valueChanged, this,
          [this](const QVariant &value) {
            t_ = value.toDouble();
            update();
          });
  anime->start();
}

void CplusplusWidget::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  const int penWidth = 1;

  auto radius = qMin(width(), height()) / 2.0 - penWidth;

  Q_ASSERT(count_ >= 0);

  // ##############坐标原点改变###############
  painter.translate(rect().center());
  QPainterPath path;
  auto radian = qDegreesToRadians(360.0 / count_);
  //    const auto _5radian = qDegreesToRadians(5.0);
  for (int i = 0; i < count_; ++i) {
    auto ww = radius * qSin(i * radian);
    auto hh = radius * qCos(i * radian);
    if (i == 0) {
      path.moveTo(ww, hh);
    } else {
      path.lineTo(ww, hh);
    }
  }
  path.closeSubpath();

  // C
  auto rCOuter = radius * 0.65;
  auto rCInner = radius * 0.27;
  QRectF rectC1(-rCOuter, -rCOuter, 2.0 * rCOuter, 2.0 * rCOuter);
  QRectF rectC2(-rCInner, -rCInner, 2.0 * rCInner, 2.0 * rCInner);

  path.moveTo(rCInner * qCos(qDegreesToRadians(30.0)),
              -rCInner * qSin(qDegreesToRadians(30.0)));
  path.lineTo(rCOuter * qCos(qDegreesToRadians(30.0)),
              -rCOuter * qSin(qDegreesToRadians(30.0)));
  path.arcTo(rectC1, 30.0, 300.0);
  path.lineTo(rCInner * qCos(qDegreesToRadians(-30.0)),
              rCInner * qSin(qDegreesToRadians(30.0)));
  path.arcTo(rectC2, -30.0, -300.0);
  path.closeSubpath();

  painter.fillPath(path, QColor(0x00549D));

  // draw ++
  painter.save();
  auto font = painter.font();
  font.setPointSizeF(22);
  painter.setFont(font);
  auto pen = painter.pen();
  pen.setColor(Qt::white);
  painter.setPen(pen);
  auto hh = painter.fontMetrics().height();
  auto ww = painter.fontMetrics().horizontalAdvance("+");

  painter.save();
  painter.translate(QPointF(rCInner - ww / 4.0, hh / 4.0));
  painter.rotate(t_);
  painter.drawText(QPointF(0, 0), "+");
  painter.restore();

  painter.save();
  painter.translate(QPointF(rCOuter - ww / 4.0, hh / 4.0));
  painter.rotate(t_);
  painter.drawText(QPointF(0, 0), "+");
  painter.restore();

  painter.restore();
}
