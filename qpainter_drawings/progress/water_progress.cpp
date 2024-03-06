#include "water_progress.h"

#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPainterPath>
#include <QSlider>
#include <QSvgRenderer>
#include <QTimer>
#include <QVBoxLayout>
#include <QtMath>

struct Bubble {
  Bubble(double s, double xs, double ys)
      : size(s), xSpeed(xs), ySpeed(ys), xOffset(0), yOffset(0) {}

  double size;
  double xSpeed;
  double ySpeed;
  double xOffset;
  double yOffset;
};

class WaterProgressPrivate : public QWidget {
 public:
  WaterProgressPrivate(WaterProgress *parent) : QWidget(parent) {
    bubbles.append(Bubble(7, -1.8, 0.6));
    bubbles.append(Bubble(8, 1.2, 1.0));
    bubbles.append(Bubble(11, 0.8, 1.6));
  }

  void resizePixmap(QSize sz);
  void initUI();
  void setValue(int v);
  void paint(QPainter *p);

  QImage waterFrontImage;
  QImage waterBackImage;
  QString progressText;
  QTimer *timer = nullptr;
  QList<Bubble> bubbles;

  int interval = 33;
  int value;

  double frontXOffset = 0;
  double backXOffset = 0;

  bool textVisible = true;
};

WaterProgress::WaterProgress(QWidget *parent) : QWidget(parent) {
  d = new WaterProgressPrivate(this);
  d->initUI();

  // apply effect
  auto effect = new QGraphicsDropShadowEffect(this);
  effect->setOffset(0, 6);
  effect->setColor(QColor(250, 215, 161, 255 * 5 / 20));
  effect->setBlurRadius(12);
  this->setGraphicsEffect(effect);
}

WaterProgress::~WaterProgress() {}

///
/// \brief WaterProgress::value
/// \return
///
int WaterProgress::value() const { return d->value; }

/*!
 * \~english \brief star water animation.
 * \~chinese \brief 开始水位动画
 */
void WaterProgress::start() { d->timer->start(); }

/*!
 * \~english \brief stop water animation.
 * \~chinese \brief 停止水位动画
 */
void WaterProgress::stop() { d->timer->stop(); }

/*!
 * \~english \brief set the current progress value (range: 0~100)
 * \~chinese \brief 设置当前进度值，（范围 0~100）
 */
void WaterProgress::setValue(int value) {
  if (d->value == value) {
    return;
  }
  d->setValue(value);
  emit valueChanged();
}

/*!
 * \~english \brief set progress text visible or not
 * \~chinese \brief 设置是否显示进度
 *
 * \~english set the progress text value(like 50% when value is 50) visible.
 * \~chinese 设置进度文字是否显示，如值为 50 时显示 50% 。
 */
void WaterProgress::setTextVisible(bool visible) { d->textVisible = visible; }

void WaterProgress::resizeEvent(QResizeEvent *) {
  int resize = qMin(width(), height());
  d->resize(resize, resize);
}

void WaterProgress::paintEvent(QPaintEvent *) {
  QPainter p(this);
  d->paint(&p);
}

void WaterProgressPrivate::resizePixmap(QSize sz) {
  // resize water;
  auto waterWidth = 500 * sz.width() / 100;
  auto waterHeight = 110 * sz.height() / 100;
  auto waterSize = QSizeF(waterWidth, waterHeight).toSize();

  if (waterFrontImage.size() != waterSize) {
    QSvgRenderer renderer(QString(":/water_progress/resource/water_front.svg"));
    QImage image(waterWidth, waterHeight, QImage::Format_ARGB32);
    image.fill(Qt::transparent);  // partly transparent red-ish background
    QPainter waterPainter(&image);
    renderer.render(&waterPainter);
    waterFrontImage = image;
  }
  if (waterBackImage.size() != waterSize) {
    QSvgRenderer renderer(QString(":/water_progress/resource/water_back.svg"));
    QImage image(waterWidth, waterHeight, QImage::Format_ARGB32);
    image.fill(Qt::transparent);  // partly transparent red-ish background
    QPainter waterPainter(&image);
    renderer.render(&waterPainter);
    waterBackImage = image;
  }
}

void WaterProgressPrivate::initUI() {
  this->setMinimumSize(100, 100);

  value = 0;

  timer = new QTimer(this);
  timer->setInterval(interval);
  resizePixmap(this->size());
  frontXOffset = this->width();
  backXOffset = 0;

  this->connect(timer, &QTimer::timeout, this, [=] {
    // interval can not be zero, and limit to 1
    interval = (interval < 1) ? 1 : interval;

    // move 60% per second
    double frontXDeta = 40.0 / (1000.0 / interval);
    // move 90% per second
    double backXDeta = 60.0 / (1000.0 / interval);

    int canvasWidth =
        static_cast<int>(this->width() * this->devicePixelRatioF());
    frontXOffset -= frontXDeta * canvasWidth / 100;
    backXOffset += backXDeta * canvasWidth / 100;

    if (frontXOffset > canvasWidth) {
      frontXOffset = canvasWidth;
    }
    if (frontXOffset < -(waterFrontImage.width() - canvasWidth)) {
      frontXOffset = canvasWidth;
    }

    if (backXOffset > waterBackImage.width()) {
      backXOffset = 0;
    }

    // update bubbles
    // move 25% per second default
    double speed = 25 / (1000.0 / interval) /** 100 / q->height()*/;
    for (auto &pop : bubbles) {
      // yOffset 0 ~ 100;
      pop.yOffset += speed * pop.ySpeed;
      if (pop.yOffset < 0) {
      }
      if (pop.yOffset > value) {
        pop.yOffset = 0;
      }
      pop.xOffset = qSin((pop.yOffset / 100) * 2 * M_PI) * 18 * pop.xSpeed + 50;
    }
    this->update();
  });
}

void WaterProgressPrivate::setValue(int v) {
  value = v;
  progressText = QString("%1%").arg(v);
  repaint();
}

void WaterProgressPrivate::paint(QPainter *p) {
  p->setRenderHint(QPainter::Antialiasing);

  qreal pixelRatio = this->devicePixelRatioF();
  QRectF rect =
      QRectF(0, 0, this->width() * pixelRatio, this->height() * pixelRatio);
  QSize sz =
      QSizeF(this->width() * pixelRatio, this->height() * pixelRatio).toSize();

  resizePixmap(sz);

  int yOffset =
      rect.toRect().topLeft().y() + (100 - value - 10) * sz.height() / 100;

  // draw water
  QImage waterImage = QImage(sz, QImage::Format_ARGB32_Premultiplied);
  QPainter waterPinter(&waterImage);
  waterPinter.setRenderHint(QPainter::Antialiasing);
  waterPinter.setCompositionMode(QPainter::CompositionMode_Source);
  waterPinter.fillRect(waterImage.rect(), QColor(43, 146, 255, 255 * 3 / 10));
  waterPinter.setCompositionMode(QPainter::CompositionMode_SourceOver);
  waterPinter.drawImage(static_cast<int>(backXOffset), yOffset, waterBackImage);
  waterPinter.drawImage(static_cast<int>(backXOffset) - waterBackImage.width(),
                        yOffset, waterBackImage);
  waterPinter.drawImage(static_cast<int>(frontXOffset), yOffset,
                        waterFrontImage);
  waterPinter.drawImage(
      static_cast<int>(frontXOffset) - waterFrontImage.width(), yOffset,
      waterFrontImage);

  // drwa pop
  if (value > 30) {
    for (auto &pop : bubbles) {
      QPainterPath popPath;
      popPath.addEllipse(pop.xOffset * sz.width() / 100,
                         (100 - pop.yOffset) * sz.height() / 100,
                         pop.size * sz.width() / 100,
                         pop.size * sz.height() / 100);
      waterPinter.fillPath(popPath, QColor(77, 208, 255));
    }
  }

  double borderWidth = 2.0 * sz.width() / 100.0;
  auto outRect = QRectF(0, 0, sz.width(), sz.height());
  QPainterPath pathBorder;
  auto factor = 0.5;
  auto margin = QMarginsF(borderWidth * factor, borderWidth * factor,
                          borderWidth * factor, borderWidth * factor);
  pathBorder.addEllipse(outRect.marginsRemoved(margin));
  waterPinter.strokePath(pathBorder,
                         QPen(QColor(252, 207, 49, 255 * 7 / 10), borderWidth));
  QPainterPath pathInnerBorder;
  auto interFactor = 1.5;
  auto innerMargin =
      QMarginsF(borderWidth * interFactor, borderWidth * interFactor,
                borderWidth * interFactor, borderWidth * interFactor);
  pathInnerBorder.addEllipse(outRect.marginsRemoved(innerMargin));
  waterPinter.strokePath(pathInnerBorder,
                         QPen(QColor(245, 85, 85, 255 * 5 / 10), borderWidth));

  if (textVisible) {
    auto font = waterPinter.font();
    font.setFamily(this->font().family());
    font.setPixelSize(sz.height() * 20 / 100);
    waterPinter.setFont(font);
    waterPinter.setPen(Qt::white);
    waterPinter.drawText(rect, Qt::AlignCenter, progressText);
  }
  waterPinter.end();

  QPixmap maskPixmap(sz);
  maskPixmap.fill(Qt::transparent);
  QPainterPath path;
  path.addEllipse(QRectF(0, 0, sz.width(), sz.height()));
  QPainter maskPainter(&maskPixmap);
  maskPainter.setRenderHint(QPainter::Antialiasing);
  maskPainter.setPen(QPen(Qt::white, 1));
  maskPainter.fillPath(path, QBrush(Qt::white));

  QPainter::CompositionMode mode = QPainter::CompositionMode_SourceIn;
  QImage contentImage = QImage(sz, QImage::Format_ARGB32_Premultiplied);
  QPainter contentPainter(&contentImage);
  contentPainter.setCompositionMode(QPainter::CompositionMode_Source);
  contentPainter.fillRect(contentImage.rect(), Qt::transparent);
  contentPainter.setCompositionMode(QPainter::CompositionMode_SourceOver);
  contentPainter.drawImage(0, 0, maskPixmap.toImage());
  contentPainter.setCompositionMode(mode);
  contentPainter.drawImage(0, 0, waterImage);
  contentPainter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
  contentPainter.end();

  contentImage.setDevicePixelRatio(pixelRatio);
  p->drawImage(this->rect(), contentImage);
}

WaterProgressDemo::WaterProgressDemo(QWidget *parent)
    : QWidget(parent),
      progress(new WaterProgress(this)),
      horizontalSlider(new QSlider(this)) {
  auto layout = new QVBoxLayout(this);
  layout->addWidget(progress);
  layout->addWidget(horizontalSlider);

  progress->setValue(50);
  progress->start();
  progress->setFont(QFont("Consolas"));

  horizontalSlider->setValue(progress->value());
  horizontalSlider->setMaximum(100);
  horizontalSlider->setOrientation(Qt::Horizontal);
  connect(horizontalSlider, &QSlider::valueChanged, progress,
          &WaterProgress::setValue);
}
