#include "speedometer.h"

#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QTimer>
#include <QtDebug>
#include <QtMath>

SpeedometerWidget::SpeedometerWidget(QWidget* parent) : QWidget(parent) {
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &SpeedometerWidget::updateValue);
  timer->start(500);
}

SpeedometerWidget::~SpeedometerWidget() {}

void SpeedometerWidget::paintEvent(QPaintEvent*) {
  int width = this->width();
  int height = this->height();
  int side = qMin(width, height);

  //绘制准备工作,启用反锯齿,平移坐标轴中心,等比例缩放
  QPainter painter(this);
  painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
  painter.translate(width / 2, height / 2);
  painter.scale(side / 200.0, side / 200.0);

  //绘制外圆
  drawOuterCircle(&painter);
  //绘制内圆
  drawInnerCircle(&painter);
  //绘制饼圆
  drawColorPie(&painter);
  //绘制覆盖圆 用以遮住饼圆多余部分
  drawCoverCircle(&painter);
  //绘制刻度线
  drawScale(&painter);
  //    return;
  //绘制刻度值
  drawScaleNum(&painter);

  //根据指示器形状绘制指示器
  if (pointerStyle == PointerStyle_Circle) {
    drawPointerCircle(&painter);
  } else if (pointerStyle == PointerStyle_Indicator) {
    drawPointerIndicator(&painter);
  } else if (pointerStyle == PointerStyle_IndicatorR) {
    drawPointerIndicatorR(&painter);
  } else if (pointerStyle == PointerStyle_Triangle) {
    drawPointerTriangle(&painter);
  }

  //绘制指针中心圆外边框
  drawRoundCircle(&painter);
  //绘制指针中心圆
  drawCenterCircle(&painter);
  //绘制当前值
  drawValue(&painter);
  //绘制遮罩层
  drawOverlay(&painter);
}

void SpeedometerWidget::drawOuterCircle(QPainter* painter) {
  int radius = 99;
  painter->save();
  painter->setPen(Qt::NoPen);
  painter->setBrush(outerCircleColor);
  painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
  painter->restore();
}

void SpeedometerWidget::drawInnerCircle(QPainter* painter) {
  int radius = 90;
  painter->save();
  painter->setPen(Qt::NoPen);
  painter->setBrush(innerCircleColor);
  painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
  painter->restore();
}

void SpeedometerWidget::drawColorPie(QPainter* painter) {
  int radius = 60;
  painter->save();
  painter->setPen(Qt::NoPen);

  QRectF rect(-radius, -radius, radius * 2, radius * 2);

  if (pieStyle == PieStyle_Three) {
    //计算总范围角度,根据占比例自动计算三色圆环范围角度
    //可以更改比例
    double angleAll = 360.0 - startAngle - endAngle;
    double angleStart = angleAll * 0.7;
    double angleMid = angleAll * 0.15;
    double angleEnd = angleAll * 0.15;

    //增加偏移量使得看起来没有脱节
    int offset = 3;

    //绘制开始饼圆
    painter->setBrush(pieColorStart);
    painter->drawPie(rect, (270 - startAngle - angleStart) * 16,
                     angleStart * 16);

    //绘制中间饼圆
    painter->setBrush(pieColorMid);
    painter->drawPie(rect,
                     (270 - startAngle - angleStart - angleMid) * 16 + offset,
                     angleMid * 16);

    //绘制结束饼圆
    painter->setBrush(pieColorEnd);
    painter->drawPie(
        rect,
        (270 - startAngle - angleStart - angleMid - angleEnd) * 16 + offset * 2,
        angleEnd * 16);
  } else if (pieStyle == PieStyle_Current) {
    //计算总范围角度,当前值范围角度,剩余值范围角度
    double angleAll = 360.0 - startAngle - endAngle;
    double angleCurrent =
        angleAll * ((currentValue - minValue) / (maxValue - minValue));
    double angleOther = angleAll - angleCurrent;

    //绘制当前值饼圆
    painter->setBrush(pieColorStart);
    painter->drawPie(rect, (270 - startAngle - angleCurrent) * 16,
                     angleCurrent * 16);

    //绘制剩余值饼圆
    painter->setBrush(pieColorEnd);
    painter->drawPie(rect, (270 - startAngle - angleCurrent - angleOther) * 16,
                     angleOther * 16);
  }

  painter->restore();
}

void SpeedometerWidget::drawCoverCircle(QPainter* painter) {
  int radius = 50;
  painter->save();
  painter->setPen(Qt::NoPen);
  painter->setBrush(coverCircleColor);
  painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
  painter->restore();
}

void SpeedometerWidget::drawScale(QPainter* painter) {
  int radius = 72;
  painter->save();

  painter->rotate(startAngle);
  int steps = (scaleMajor * scaleMinor);
  double angleStep = (360.0 - startAngle - endAngle) / steps;

  QPen pen;
  pen.setColor(scaleColor);
  pen.setCapStyle(Qt::RoundCap);

  for (int i = 0; i <= steps; i++) {
    if (i % scaleMinor == 0) {
      pen.setWidthF(1.5);
      painter->setPen(pen);
      painter->drawLine(0, radius - 10, 0, radius);
    } else {
      pen.setWidthF(0.5);
      painter->setPen(pen);
      painter->drawLine(0, radius - 5, 0, radius);
    }

    painter->rotate(angleStep);
  }

  painter->restore();
}

void SpeedometerWidget::drawScaleNum(QPainter* painter) {
  int radius = 82;
  painter->save();
  painter->setPen(scaleColor);

  double startRad = (360 - startAngle - 90) * (M_PI / 180);
  double deltaRad = (360 - startAngle - endAngle) * (M_PI / 180) / scaleMajor;

  for (int i = 0; i <= scaleMajor; i++) {
    double sina = qSin(startRad - i * deltaRad);
    double cosa = qCos(startRad - i * deltaRad);
    double value = 1.0 * i * ((maxValue - minValue) / scaleMajor) + minValue;

    QString strValue = QString("%1").arg((double)value, 0, 'f', precision);
    double textWidth = fontMetrics().width(strValue);
    double textHeight = fontMetrics().height();
    int x = radius * cosa - textWidth / 2;
    int y = -radius * sina + textHeight / 4;
    painter->drawText(x, y, strValue);
  }

  painter->restore();
}

void SpeedometerWidget::drawPointerCircle(QPainter* painter) {
  int radius = 6;
  int offset = 30;
  painter->save();
  painter->setPen(Qt::NoPen);
  painter->setBrush(pointerColor);

  painter->rotate(startAngle);
  double degRotate = (360.0 - startAngle - endAngle) / (maxValue - minValue) *
                     (currentValue - minValue);
  painter->rotate(degRotate);
  painter->drawEllipse(-radius, radius + offset, radius * 2, radius * 2);

  painter->restore();
}

void SpeedometerWidget::drawPointerIndicator(QPainter* painter) {
  int radius = 75;
  painter->save();
  painter->setOpacity(0.8);
  painter->setPen(Qt::NoPen);
  painter->setBrush(pointerColor);

  QPolygon pts;
  pts.setPoints(3, -5, 0, 5, 0, 0, radius);

  painter->rotate(startAngle);
  double degRotate = (360.0 - startAngle - endAngle) / (maxValue - minValue) *
                     (currentValue - minValue);
  painter->rotate(degRotate);
  painter->drawConvexPolygon(pts);

  painter->restore();
}

void SpeedometerWidget::drawPointerIndicatorR(QPainter* painter) {
  int radius = 75;
  painter->save();
  painter->setOpacity(1.0);

  QPen pen;
  pen.setWidth(1);
  pen.setColor(pointerColor);
  painter->setPen(pen);
  painter->setBrush(pointerColor);

  QPolygon pts;
  pts.setPoints(3, -5, 0, 5, 0, 0, radius);

  painter->rotate(startAngle);
  double degRotate = (360.0 - startAngle - endAngle) / (maxValue - minValue) *
                     (currentValue - minValue);
  painter->rotate(degRotate);
  painter->drawConvexPolygon(pts);

  //增加绘制圆角直线,与之前三角形重叠,形成圆角指针
  pen.setCapStyle(Qt::RoundCap);
  pen.setWidthF(4);
  painter->setPen(pen);
  painter->drawLine(0, 0, 0, radius);

  painter->restore();
}

void SpeedometerWidget::drawPointerTriangle(QPainter* painter) {
  int radius = 10;
  int offset = 38;
  painter->save();
  painter->setPen(Qt::NoPen);
  painter->setBrush(pointerColor);

  QPolygon pts;
  pts.setPoints(3, -5, 0 + offset, 5, 0 + offset, 0, radius + offset);

  painter->rotate(startAngle);
  double degRotate = (360.0 - startAngle - endAngle) / (maxValue - minValue) *
                     (currentValue - minValue);
  painter->rotate(degRotate);
  painter->drawConvexPolygon(pts);

  painter->restore();
}

void SpeedometerWidget::drawRoundCircle(QPainter* painter) {
  int radius = 18;
  painter->save();
  painter->setOpacity(0.8);
  painter->setPen(Qt::NoPen);
  painter->setBrush(pointerColor);
  painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
  painter->restore();
}

void SpeedometerWidget::drawCenterCircle(QPainter* painter) {
  int radius = 15;
  painter->save();
  painter->setPen(Qt::NoPen);
  painter->setBrush(centerCircleColor);
  painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
  painter->restore();
}

void SpeedometerWidget::drawValue(QPainter* painter) {
  int radius = 100;
  painter->save();
  painter->setPen(textColor);

  QFont font;
  font.setPixelSize(18);
  painter->setFont(font);

  QRectF textRect(-radius, -radius, radius * 2, radius * 2);
  QString strValue = QString("%1").arg((double)currentValue, 0, 'f', precision);
  painter->drawText(textRect, Qt::AlignCenter, strValue);

  painter->restore();
}

void SpeedometerWidget::drawOverlay(QPainter* painter) {
  if (!showOverlay) {
    return;
  }

  int radius = 90;
  painter->save();
  painter->setPen(Qt::NoPen);

  QPainterPath smallCircle;
  QPainterPath bigCircle;
  radius -= 1;
  smallCircle.addEllipse(-radius, -radius, radius * 2, radius * 2);
  radius *= 2;
  bigCircle.addEllipse(-radius, -radius + 140, radius * 2, radius * 2);

  //高光的形状为小圆扣掉大圆的部分
  QPainterPath highlight = smallCircle - bigCircle;

  QLinearGradient linearGradient(0, -radius / 2, 0, 0);
  overlayColor.setAlpha(100);
  linearGradient.setColorAt(0.0, overlayColor);
  overlayColor.setAlpha(30);
  linearGradient.setColorAt(1.0, overlayColor);
  painter->setBrush(linearGradient);
  painter->rotate(-20);
  painter->drawPath(highlight);

  painter->restore();
}

void SpeedometerWidget::updateValue() {
  update();
}

double SpeedometerWidget::getMinValue() const {
  return minValue;
}

double SpeedometerWidget::getMaxValue() const {
  return maxValue;
}

double SpeedometerWidget::getValue() const {
  return this->value;
}

int SpeedometerWidget::getPrecision() const {
  return this->precision;
}

int SpeedometerWidget::getScaleMajor() const {
  return this->scaleMajor;
}

int SpeedometerWidget::getScaleMinor() const {
  return this->scaleMinor;
}

int SpeedometerWidget::getStartAngle() const {
  return this->startAngle;
}

int SpeedometerWidget::getEndAngle() const {
  return this->endAngle;
}

bool SpeedometerWidget::getAnimation() const {
  return this->animation;
}

double SpeedometerWidget::getAnimationStep() const {
  return this->animationStep;
}

QColor SpeedometerWidget::getOuterCircleColor() const {
  return this->outerCircleColor;
}

QColor SpeedometerWidget::getInnerCircleColor() const {
  return this->innerCircleColor;
}

QColor SpeedometerWidget::getPieColorStart() const {
  return this->pieColorStart;
}

QColor SpeedometerWidget::getPieColorMid() const {
  return this->pieColorMid;
}

QColor SpeedometerWidget::getPieColorEnd() const {
  return this->pieColorEnd;
}

QColor SpeedometerWidget::getCoverCircleColor() const {
  return this->coverCircleColor;
}

QColor SpeedometerWidget::getScaleColor() const {
  return this->scaleColor;
}

QColor SpeedometerWidget::getPointerColor() const {
  return this->pointerColor;
}

QColor SpeedometerWidget::getCenterCircleColor() const {
  return this->centerCircleColor;
}

QColor SpeedometerWidget::getTextColor() const {
  return this->textColor;
}

bool SpeedometerWidget::getShowOverlay() const {
  return this->showOverlay;
}

QColor SpeedometerWidget::getOverlayColor() const {
  return this->overlayColor;
}

SpeedometerWidget::PieStyle SpeedometerWidget::getPieStyle() const {
  return this->pieStyle;
}

SpeedometerWidget::PointerStyle SpeedometerWidget::getPointerStyle() const {
  return this->pointerStyle;
}

QSize SpeedometerWidget::sizeHint() const {
  return QWidget::sizeHint();
}

QSize SpeedometerWidget::minimumSizeHint() const {
  return QWidget::minimumSizeHint();
}

void SpeedometerWidget::setRange(double minValue, double maxValue) {
  this->minValue = minValue;
  this->maxValue = maxValue;
}

void SpeedometerWidget::setRange(int minValue, int maxValue) {
  this->minValue = minValue;
  this->maxValue = maxValue;
}

void SpeedometerWidget::setMinValue(double minValue) {
  this->minValue = minValue;
}

void SpeedometerWidget::setMaxValue(double maxValue) {
  this->maxValue = maxValue;
}

void SpeedometerWidget::setValue(double value) {
  this->value = value;
}

void SpeedometerWidget::setValue(int value) {
  this->value = value;
}

void SpeedometerWidget::setPrecision(int precision) {
  this->precision = precision;
}

void SpeedometerWidget::setScaleMajor(int scaleMajor) {
  this->scaleMajor = scaleMajor;
}

void SpeedometerWidget::setScaleMinor(int scaleMinor) {
  this->scaleMinor = scaleMinor;
}

void SpeedometerWidget::setStartAngle(int startAngle) {
  this->startAngle = startAngle;
}

void SpeedometerWidget::setEndAngle(int endAngle) {
  this->endAngle = endAngle;
}

void SpeedometerWidget::setAnimation(bool animation) {
  this->animation = animation;
}

void SpeedometerWidget::setAnimationStep(double animationStep) {
  this->animationStep = animationStep;
}

void SpeedometerWidget::setOuterCircleColor(const QColor& outerCircleColor) {
  this->outerCircleColor = outerCircleColor;
}

void SpeedometerWidget::setInnerCircleColor(const QColor& innerCircleColor) {
  this->innerCircleColor = innerCircleColor;
}

void SpeedometerWidget::setPieColorStart(const QColor& pieColorStart) {
  this->pieColorStart = pieColorStart;
}

void SpeedometerWidget::setPieColorMid(const QColor& pieColorMid) {
  this->pieColorMid = pieColorMid;
}

void SpeedometerWidget::setPieColorEnd(const QColor& pieColorEnd) {
  this->pieColorEnd = pieColorEnd;
}

void SpeedometerWidget::setCoverCircleColor(const QColor& coverCircleColor) {
  this->coverCircleColor = coverCircleColor;
}

void SpeedometerWidget::setScaleColor(const QColor& scaleColor) {
  this->scaleColor = scaleColor;
}

void SpeedometerWidget::setPointerColor(const QColor& pointerColor) {
  this->pointerColor = pointerColor;
}

void SpeedometerWidget::setCenterCircleColor(const QColor& centerCircleColor) {
  this->centerCircleColor = centerCircleColor;
}

void SpeedometerWidget::setTextColor(const QColor& textColor) {
  this->textColor = textColor;
}

void SpeedometerWidget::setShowOverlay(bool showOverlay) {
  this->showOverlay = showOverlay;
}

void SpeedometerWidget::setOverlayColor(const QColor& overlayColor) {
  this->overlayColor = overlayColor;
}

void SpeedometerWidget::setPieStyle(
    const SpeedometerWidget::PieStyle& pieStyle) {
  this->pieStyle = pieStyle;
}

void SpeedometerWidget::setPointerStyle(
    const SpeedometerWidget::PointerStyle& pointerStyle) {
  this->pointerStyle = pointerStyle;
}
