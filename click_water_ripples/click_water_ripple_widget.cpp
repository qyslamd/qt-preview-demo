#include "click_water_ripple_widget.h"

#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QVariantAnimation>
#include <QtDebug>

class WaterRipplePrivate {
 public:
  WaterRipplePrivate(ClickWaterRipple *p) : anime_(new QVariantAnimation(p)) {}

 public:
  QVariantAnimation *anime_ = nullptr;
  QPoint start_pos;
  QRect rect_;
  qreal radius_;
};

ClickWaterRipple::ClickWaterRipple(QWidget *parent)
    : QWidget(parent),
      d_(new WaterRipplePrivate(this))

{
  connect(d_->anime_, &QVariantAnimation::valueChanged, this,
          [this](const QVariant &value) {
            d_->radius_ = value.toDouble();
            update();
          });
  connect(d_->anime_, &QVariantAnimation::finished, this, [this]() {
    d_->radius_ = 0;
    update();
  });
}

ClickWaterRipple::~ClickWaterRipple() { delete d_; }

void ClickWaterRipple::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.fillRect(rect(), Qt::white);

  if (d_->anime_->state() != QAbstractAnimation::Running) {
    return;
  }
  QPainterPath path;

  QRectF rect(0, 0, d_->radius_, d_->radius_);
  rect.moveCenter(d_->start_pos);
  path.addEllipse(rect);

  painter.save();
  QLinearGradient linearGrad(rect.topLeft(), rect.bottomRight());
  linearGrad.setColorAt(0, QColor(0x7085b6));
  linearGrad.setColorAt(0.5, QColor(0x87a7d9));
  linearGrad.setColorAt(1, QColor(0xdef3f8));
  painter.fillPath(path, linearGrad);
  painter.restore();
}

void ClickWaterRipple::mousePressEvent(QMouseEvent *event) {
  d_->start_pos = event->pos();
  d_->anime_->setKeyValueAt(0, 10);
  d_->anime_->setKeyValueAt(1.0, 2 * qMax(width(), height()));
  d_->anime_->setDuration(400);
  d_->anime_->setEasingCurve(QEasingCurve::InCubic);
  d_->anime_->start();
}
