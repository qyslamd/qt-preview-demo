#include "wifi_logo.h"

#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QTimer>
#include <QtDebug>
#include <cmath>
#include <future>
#include <mutex>
#include <thread>

static void GetWifiPixmap(QPixmap &pix, qreal strength,
                          bool authorize = false) {
  static const auto spanAngle = 90 * 16;
  static const auto startAngle = 45 * 16;

  pix.fill(Qt::transparent);

  QPainter painter(&pix);

  painter.save();
  painter.setRenderHint(QPainter::Antialiasing);

  auto minWidth = std::min(pix.width(), pix.height());
  auto penWidth = 60.0 / 500.0 * minWidth;

  auto yOffset = (415.0 - 256.0) / 512.0 * minWidth;
  painter.translate(pix.width() / 2.0, pix.height() / 2.0 + yOffset);

  // 39,38,54
  QColor signalColor(Qt::white), unSignalColor(148, 148, 148);

  // 75% - 100%
  auto radius = std::sqrt(minWidth * minWidth / 2.0);
  QRectF rectArc(-radius + penWidth / 2.0, -radius + penWidth / 2.0,
                 2 * radius - penWidth, 2 * radius - penWidth);
  painter.save();
  painter.setPen(QPen(strength > 0.75 ? signalColor : unSignalColor, penWidth,
                      Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
  painter.drawArc(rectArc, startAngle, spanAngle);
  painter.restore();

  // 50% - 75%
  auto redius2 = radius * 2.0 / 3.0;
  rectArc = QRectF(-redius2 + penWidth / 2.0, -redius2 + penWidth / 2.0,
                   2 * redius2 - penWidth, 2 * redius2 - penWidth);
  painter.save();
  painter.setPen(QPen(strength > 0.5 ? signalColor : unSignalColor, penWidth,
                      Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
  painter.drawArc(rectArc, startAngle, spanAngle);
  painter.restore();

  // 是否需要认证
  if (authorize) {
    painter.save();
    painter.setPen(QPen(signalColor, penWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(rectArc.left(), rectArc.top(), rectArc.left(),
                     rectArc.top() + rectArc.height() * 1.0 / 3.0);
    painter.restore();

    painter.save();
    painter.setPen(QPen(signalColor, penWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawPoint(
        rectArc.left(),
        rectArc.top() + rectArc.height() * 1.0 / 3.0 + penWidth * 1.5);
    painter.restore();
  }

  // 0% - 50%
  auto redius3 = radius * 1.0 / 3.0;
  rectArc = QRectF(-redius3 + penWidth / 2.0, -redius3 + penWidth / 2.0,
                   2 * redius3 - penWidth, 2 * redius3 - penWidth);
  painter.save();
  painter.setPen(QPen(strength > 0.1 ? signalColor : unSignalColor, penWidth,
                      Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
  painter.drawArc(rectArc, startAngle, spanAngle);
  painter.restore();

  // point
  painter.save();
  painter.setPen(
      QPen(signalColor, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  painter.drawPoint(0, 0);
  painter.restore();

  painter.restore();
}

TestWifiLogo::TestWifiLogo(QWidget *parent) : QWidget(parent) {
  auto timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, [this] {
    ratio += 0.2;
    if (ratio > 1.0) {
      ratio = 0;
    }

    QPixmap pix(width(), height());
    std::async(std::launch::async, GetWifiPixmap, std::ref(pix), ratio,
               std::floorl(ratio * 100) < 50)
        .get();
    pixmap = pix;

    update();
  });
  timer->start(500);
}

void TestWifiLogo::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.fillRect(rect(), QColor(64, 65, 66));

  painter.save();
  painter.drawPixmap(rect(), pixmap, pixmap.rect());
  painter.restore();
}
