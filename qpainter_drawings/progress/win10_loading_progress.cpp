#include "win10_loading_progress.h"

#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QVariantAnimation>
#include <QtDebug>

Win10LoadingProgressWidget::Win10LoadingProgressWidget(QWidget* parent)
    : QWidget(parent), anime_(new QVariantAnimation(this)) {
  anime_->setStartValue(0.0);
  anime_->setEndValue(1.0);
  anime_->setDuration(3000);
  anime_->setLoopCount(-1);
  anime_->setEasingCurve(QEasingCurve::InOutCubic);
  connect(anime_, &QVariantAnimation::valueChanged,
          [this](const QVariant& value) {
            auto tt = value.toDouble();
            t_ = tt;
            update();
          });
  anime_->start();
}

Win10LoadingProgressWidget::~Win10LoadingProgressWidget() {}

void Win10LoadingProgressWidget::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);

  QPainter p(this);

  qreal ratio = .15;
  auto ww = width() * ratio;
  auto hh = height() * ratio;
  auto margin = std::min(ww, hh);
  auto rect = this->rect().adjusted(margin, margin, -margin, -margin);
  auto radius = std::min(rect.width(), rect.height());
  rect.setWidth(radius);
  rect.setHeight(radius);

  QPainterPath path;
  path.moveTo(rect.center().x(), rect.top());
  path.arcTo(rect, 90, -360);

  p.save();
  p.setRenderHint(QPainter::Antialiasing);
  p.setPen(QPen(QBrush(QColor(Qt::red)), 15, Qt::SolidLine, Qt::RoundCap));

  auto p0 = path.pointAtPercent(t_);
  p.drawPoint(p0);

  auto delta = t_ - 0.05 * (1 - t_);
  if (delta > 0) {
    auto pos = path.pointAtPercent(delta);
    p.drawPoint(pos);
  }

  delta = t_ - 0.1 * (1 - t_);
  if (delta > 0) {
    auto pos = path.pointAtPercent(delta);
    p.drawPoint(pos);
  }

  delta = t_ - 0.15 * (1 - t_);
  if (delta > 0) {
    auto pos = path.pointAtPercent(delta);
    p.drawPoint(pos);
  }

  if (t_ > 0.98) {
    p.drawPoint(path.pointAtPercent(1));
  }

  p.restore();
}

qreal Win10LoadingProgressWidget::currentValue() {
  return anime_->currentValue().toDouble();
}
