#include "bagua_round.h"

#include <QConicalGradient>
#include <QHoverEvent>
#include <QLinearGradient>
#include <QPainter>
#include <QPainterPath>
#include <QStaticText>
#include <QTimer>
#include <QtDebug>
#include <cmath>

static auto angle_8_1 = 360.0 / 8.0;
static auto angle_24_1 = 360.0 / 24.0;
static auto angle_32_1 = 360.0 / 32.0;

class RotateData {
  BaguaRound *p_ = nullptr;

 public:
  RotateData(BaguaRound *p) : p_(p) {}
  qreal fish_degree_;
  qreal gua_degree_;
  qreal _24_soloar_degree_;
};

BaguaRound::BaguaRound(QWidget *parent)
    : QOpenGLWidget(parent), r_d_(new RotateData(this)) {
  QSurfaceFormat surfaceFormat;
  surfaceFormat.setSamples(4);
  setFormat(surfaceFormat);

  resize(600, 600);

  QConicalGradient coneGradient(0, 0, 180.0);
  coneGradient.setColorAt(0.00, QColor(35, 40, 3, 255));
  coneGradient.setColorAt(0.160, QColor(136, 106, 22, 255));
  coneGradient.setColorAt(0.225, QColor(166, 140, 41, 255));
  coneGradient.setColorAt(0.285, QColor(204, 181, 74, 255));
  coneGradient.setColorAt(0.345, QColor(235, 219, 102, 255));
  coneGradient.setColorAt(0.415, QColor(245, 236, 112, 255));
  coneGradient.setColorAt(0.520, QColor(209, 190, 76, 255));
  coneGradient.setColorAt(0.570, QColor(187, 156, 51, 255));
  coneGradient.setColorAt(0.635, QColor(168, 142, 42, 255));
  coneGradient.setColorAt(0.695, QColor(202, 174, 68, 255));
  coneGradient.setColorAt(0.75, QColor(218, 202, 86, 255));
  coneGradient.setColorAt(0.815, QColor(208, 187, 73, 255));
  coneGradient.setColorAt(0.88, QColor(187, 156, 51, 255));
  coneGradient.setColorAt(0.935, QColor(137, 108, 26, 255));
  coneGradient.setColorAt(1, QColor(35, 40, 3, 255));
  background_brush_ = QBrush(coneGradient);

  //    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
  //    setAttribute(Qt::WA_TranslucentBackground);

  auto timer = new QTimer(this);
  connect(timer, &QTimer::timeout, [this] {
    r_d_->fish_degree_ -= 15;
    r_d_->gua_degree_ += 15;
    r_d_->_24_soloar_degree_ += 30;
    update();
  });
  timer->start(30);
}

void BaguaRound::paintEvent(QPaintEvent *event) {
  QOpenGLWidget::paintEvent(event);

  QPainter painter(this);
  painter.fillRect(rect(), QColor(Qt::white));  // 0,0,0,1

  painter.setRenderHints(QPainter::Antialiasing);
  // 移动当前的坐标系到绘制的中心，这样便于坐标计算
  painter.translate(QPointF(this->width() / 2.0, this->height() / 2.0));
  auto radius = std::min(this->width(), this->height()) / 2.0;

  // 从外层到内的圆环占据整个绘制的比例
  auto radius_0 = radius * (300.0 - 5.000) / 300.0;
  auto radius_0_1 = radius * (300.0 - 35.000) / 300.0;
  auto radius_1 = radius * (300.0 - 60.00) / 300.0;
  auto radius_2 = radius * (300.0 - 90.00) / 300.0;
  auto radius_3 = radius * (300.0 - 120.0) / 300.0;
  auto radius_4 = radius * (300.0 - 150.0) / 300.0;
  auto radius_5 = radius * (300.0 - 180.0) / 300.0;
  auto radius_6 = radius * (300.0 - 210.0) / 300.0;
  auto radius_7 = radius * (300.0 - 240.0) / 300.0;
  auto radius_fish = radius * (300.0 - 280.0) / 300.0;

  drawBackground(&painter, radius_0_1);
  drawYinYangFish(&painter, radius_fish);
  drawGua(&painter, radius_6);
  drawGuaCharacter(&painter, radius_5, radius_6);
  drawTianGanWith5Elements(&painter, radius_4, radius_5);
  drawDiZhiWith5Elements(&painter, radius_3, radius_4);
  drawTianGanDiZhi(&painter, radius_2, radius_3);
  draw24SolarTerms(&painter, radius_1, radius_2);
  drawScale(&painter, radius_0, radius_0_1);
  drawCircle(&painter, radius_0, QColor(0x000001), 1.0);
  drawCircle(&painter, radius_0_1, QColor(0x6E5E26), 2.0);
  drawCircle(&painter, radius_1, QColor(0x834447), 2.0);
  drawCircle(&painter, radius_2, QColor(0x000001), 2.0);
  drawCircle(&painter, radius_3, QColor(0xDC143C), 2.0);
  drawCircle(&painter, radius_4, QColor(0x808000), 2.0);
  drawCircle(&painter, radius_5, QColor(0x808000), 2.0);
  drawCircle(&painter, radius_7, QColor(0x834447), 2.0);
  drawOneEighthSeperator(&painter, radius_1, radius_7);
  drawOneTwentyFourthSeperator(&painter, radius_1, radius_3);
  drawQuarterSeperator(&painter, radius_3, radius_4);
}

void BaguaRound::showEvent(QShowEvent *event) {
  QOpenGLWidget::showEvent(event);
  //    QPixmap pix(1920,1920);
  //    getBaguaPixmap(pix);
  //    if(pix.save(R"(C:\Users\slamdd\Desktop\bagua.png)", "PNG",100))
  //    {
  //        qInfo()<<"ok";
  //    }else{
  //        qInfo()<<"failed";
  //    }
}

void BaguaRound::getBaguaPixmap(QPixmap &pixmap) {
  if (pixmap.width() == 0 || pixmap.height() == 0) {
    return;
  }

  QPainter painter(&pixmap);
  painter.setRenderHints(QPainter::Antialiasing);
  // 移动当前的坐标系到绘制的中心，这样便于坐标计算
  painter.translate(QPointF(pixmap.width() / 2.0, pixmap.height() / 2.0));
  auto radius = std::min(pixmap.width(), pixmap.height()) / 2.0;

  // 从外层到内的圆环占据整个绘制的比例
  auto radius_0 = radius * (300.0 - 5.000) / 300.0;
  auto radius_0_1 = radius * (300.0 - 35.000) / 300.0;
  auto radius_1 = radius * (300.0 - 60.00) / 300.0;
  auto radius_2 = radius * (300.0 - 90.00) / 300.0;
  auto radius_3 = radius * (300.0 - 120.0) / 300.0;
  auto radius_4 = radius * (300.0 - 150.0) / 300.0;
  auto radius_5 = radius * (300.0 - 180.0) / 300.0;
  auto radius_6 = radius * (300.0 - 210.0) / 300.0;
  auto radius_7 = radius * (300.0 - 240.0) / 300.0;
  auto radius_fish = radius * (300.0 - 280.0) / 300.0;

  drawBackground(&painter, radius_0_1);
  drawYinYangFish(&painter, radius_fish);
  drawGua(&painter, radius_6);
  drawGuaCharacter(&painter, radius_5, radius_6);
  drawTianGanWith5Elements(&painter, radius_4, radius_5);
  drawDiZhiWith5Elements(&painter, radius_3, radius_4);
  drawTianGanDiZhi(&painter, radius_2, radius_3);
  draw24SolarTerms(&painter, radius_1, radius_2);
  drawScale(&painter, radius_0, radius_0_1);
  drawCircle(&painter, radius_0, QColor(0x000001), 1.0);
  drawCircle(&painter, radius_0_1, QColor(0x6E5E26), 2.0);
  drawCircle(&painter, radius_1, QColor(0x834447), 2.0);
  drawCircle(&painter, radius_2, QColor(0x000001), 2.0);
  drawCircle(&painter, radius_3, QColor(0xDC143C), 2.0);
  drawCircle(&painter, radius_4, QColor(0x808000), 2.0);
  drawCircle(&painter, radius_5, QColor(0x808000), 2.0);
  drawCircle(&painter, radius_7, QColor(0x834447), 2.0);
  drawOneEighthSeperator(&painter, radius_1, radius_7);
  drawOneTwentyFourthSeperator(&painter, radius_1, radius_3);
  drawQuarterSeperator(&painter, radius_3, radius_4);
}

void BaguaRound::drawBackground(QPainter *painter, qreal radius) {
  painter->save();
  painter->setBrush(background_brush_);
  painter->setPen(Qt::NoPen);
  painter->drawEllipse(QRectF(-radius, -radius, 2.0 * radius, 2.0 * radius));
  painter->restore();
}

void BaguaRound::drawYinYangFish(QPainter *painter, qreal radius) {
  painter->save();
  painter->rotate(r_d_->fish_degree_);

  auto diameter = 2.0 * radius;
  painter->save();
  painter->setPen(Qt::black);
  painter->setBrush(Qt::black);
  painter->drawEllipse(QRectF(-radius, -radius, diameter, diameter));
  painter->restore();

  // white fish head
  auto fishHeadRadius = radius / 2.0f;
  QPainterPath path(QPointF(0.0f, -radius));
  path.arcTo(QRectF(-radius, -radius, diameter, diameter), 90.0, 180.0);
  path.arcTo(QRectF(-fishHeadRadius, 0.0, radius, radius), -90.0, -180.0);
  path.arcTo(QRectF(-fishHeadRadius, -radius, radius, radius), -90.0, 180.0);
  painter->save();
  painter->setPen(QPen(QColor(Qt::black), 2.0));
  painter->drawPath(path);  // he he
  painter->fillPath(path, Qt::white);
  painter->restore();

  // white fish eye
  auto fishEyeWidth = radius / 2.5;
  painter->save();
  painter->setPen(QPen(QBrush(Qt::black), fishEyeWidth, Qt::SolidLine,
                       Qt::RoundCap, Qt::RoundJoin));
  painter->drawPoint(QPointF(0.0, -radius / 2.0));
  painter->restore();

  // black fish eye
  painter->save();
  painter->setPen(QPen(QBrush(Qt::white), fishEyeWidth, Qt::SolidLine,
                       Qt::RoundCap, Qt::RoundJoin));
  painter->drawPoint(QPointF(0.0, radius / 2.0));
  painter->restore();

  painter->restore();
}

void BaguaRound::drawGua(QPainter *painter, qreal radius) {
  painter->save();
  painter->rotate(r_d_->gua_degree_);

  auto penWidth = 4.0;
  static auto spacerAngle = angle_8_1 / 8.0;
  static auto rightStartAngle =
      (angle_8_1 + angle_8_1 / 2.0 + spacerAngle) * 16;
  static auto leftStartAngle = (90.0 + spacerAngle) * 16;
  static auto spanAngleFull = (angle_8_1 - 2 * spacerAngle) * 16;
  static auto spanAngleHalf = (angle_8_1 / 2.0 - 2 * spacerAngle) * 16;

  auto drawer = [=](qreal angle, std::tuple<bool, QColor, QColor> top,
                    std::tuple<bool, QColor, QColor> middle,
                    std::tuple<bool, QColor, QColor> bottom) {
    painter->save();
    painter->rotate(angle);

    // top
    if (std::get<0>(top)) {
      painter->save();
      painter->setPen(QPen(QBrush(std::get<1>(top)), penWidth, Qt::SolidLine,
                           Qt::RoundCap, Qt::RoundJoin));
      painter->drawArc(QRectF(-radius, -radius, 2 * radius, 2 * radius),
                       rightStartAngle, spanAngleFull);
      painter->restore();
    } else {
      painter->save();
      painter->setPen(QPen(QBrush(std::get<1>(top)), penWidth, Qt::SolidLine,
                           Qt::RoundCap, Qt::MiterJoin));
      painter->drawArc(QRectF(-radius, -radius, 2 * radius, 2 * radius),
                       rightStartAngle, spanAngleHalf);
      painter->restore();

      painter->save();
      painter->setPen(QPen(QBrush(std::get<2>(top)), penWidth, Qt::SolidLine,
                           Qt::RoundCap, Qt::MiterJoin));
      painter->drawArc(QRectF(-radius, -radius, 2 * radius, 2 * radius),
                       leftStartAngle, spanAngleHalf);
      painter->restore();
    }

    // middle
    auto radius_temp = radius - 2.0 * penWidth;
    if (std::get<0>(middle)) {
      painter->save();
      painter->setPen(QPen(QBrush(std::get<1>(middle)), penWidth, Qt::SolidLine,
                           Qt::RoundCap, Qt::RoundJoin));
      painter->drawArc(
          QRectF(-radius_temp, -radius_temp, 2 * radius_temp, 2 * radius_temp),
          rightStartAngle, spanAngleFull);
      painter->restore();
    } else {
      painter->save();
      painter->setPen(QPen(QBrush(std::get<1>(middle)), penWidth, Qt::SolidLine,
                           Qt::RoundCap, Qt::MiterJoin));
      painter->drawArc(
          QRectF(-radius_temp, -radius_temp, 2 * radius_temp, 2 * radius_temp),
          rightStartAngle, spanAngleHalf);
      painter->restore();

      painter->save();
      painter->setPen(QPen(QBrush(std::get<2>(middle)), penWidth, Qt::SolidLine,
                           Qt::RoundCap, Qt::MiterJoin));
      painter->drawArc(
          QRectF(-radius_temp, -radius_temp, 2 * radius_temp, 2 * radius_temp),
          leftStartAngle, spanAngleHalf);
      painter->restore();
    }

    // bottom
    radius_temp = radius - 4.0 * penWidth;
    if (std::get<0>(bottom)) {
      painter->save();
      painter->setPen(QPen(QBrush(std::get<1>(bottom)), penWidth, Qt::SolidLine,
                           Qt::RoundCap, Qt::RoundJoin));
      painter->drawArc(
          QRectF(-radius_temp, -radius_temp, 2 * radius_temp, 2 * radius_temp),
          rightStartAngle, spanAngleFull);
      painter->restore();
    } else {
      painter->save();
      painter->setPen(QPen(QBrush(std::get<1>(bottom)), penWidth, Qt::SolidLine,
                           Qt::RoundCap, Qt::MiterJoin));
      painter->drawArc(
          QRectF(-radius_temp, -radius_temp, 2 * radius_temp, 2 * radius_temp),
          rightStartAngle, spanAngleHalf);
      painter->restore();

      painter->save();
      painter->setPen(QPen(QBrush(std::get<2>(bottom)), penWidth, Qt::SolidLine,
                           Qt::RoundCap, Qt::MiterJoin));
      painter->drawArc(
          QRectF(-radius_temp, -radius_temp, 2 * radius_temp, 2 * radius_temp),
          leftStartAngle, spanAngleHalf);
      painter->restore();
    }

    painter->restore();  // restore for rotate
  };

  /*-------------------------
   * 离中虚
   * */
  drawer(angle_8_1 * 0.0, std::make_tuple(true, QColor(0xDC143C), QColor()),
         std::make_tuple(false, QColor(0xFFFF00), QColor(0xFFD700)),
         std::make_tuple(true, QColor(0xF1D24E), QColor()));

  /*-------------------------
   * 坤六断
   * */
  drawer(angle_8_1 * 1.0,
         std::make_tuple(false, QColor(0xBC211E), QColor(0xEAB529)),
         std::make_tuple(false, QColor(0xADFF2F), QColor(0x7CFC00)),
         std::make_tuple(false, QColor(0xEEE8AA), QColor(0xF0E68C)));

  /*-------------------------
   * 兑上缺
   * */
  drawer(angle_8_1 * 2.0,
         std::make_tuple(false, QColor(0x40E0D0), QColor(0x7FFFAA)),
         std::make_tuple(true, QColor(0x00FA9A), QColor()),
         std::make_tuple(true, QColor(0x00FF7F), QColor()));

  /*-------------------------
   * 乾三连
   * */
  drawer(angle_8_1 * 3.0, std::make_tuple(true, QColor(0x87CEFA), QColor()),
         std::make_tuple(true, QColor(0x87CEEB), QColor()),
         std::make_tuple(true, QColor(0x00BFFF), QColor()));

  /*-------------------------
   * 坎中满
   * */
  drawer(angle_8_1 * 4.0,
         std::make_tuple(false, QColor(0xB0E0E6), QColor(0xB0E0E6)),
         std::make_tuple(true, QColor(0x5F9EA0), QColor()),
         std::make_tuple(false, QColor(0x008B8B), QColor(0x00CED1)));

  /*-------------------------
   * 艮覆碗
   * */
  drawer(angle_8_1 * 5.0, std::make_tuple(true, QColor(0xBA55D3), QColor()),
         std::make_tuple(false, QColor(0x9400D3), QColor(0x9932CC)),
         std::make_tuple(false, QColor(0xBA55D3), QColor(0x9370DB)));

  /*-------------------------
   * 震仰盂
   * */
  drawer(angle_8_1 * 6.0,
         std::make_tuple(false, QColor(0xDDA0DD), QColor(0xEE82EE)),
         std::make_tuple(false, QColor(0xFF00FF), QColor(0xFF00FF)),
         std::make_tuple(true, QColor(0xDA70D6), QColor()));

  /*-------------------------
   * 巽下断
   * */
  drawer(angle_8_1 * 7.0, std::make_tuple(true, QColor(0xFF69B4), QColor()),
         std::make_tuple(true, QColor(0xFF1493), QColor()),
         std::make_tuple(false, QColor(0xFFB6C1), QColor(0xFFC0CB)));

  painter->restore();
}

void BaguaRound::drawGuaCharacter(QPainter *painter, qreal radius_outer,
                                  qreal radius_inner) {
  auto height = radius_outer - radius_inner;
  auto fontSizePixel = height * 3.0 / 5.0;
  auto drawer = [=](qreal degree, const QString &character,
                    const QColor &color = QColor()) {
    painter->save();
    painter->rotate(degree);
    auto font = painter->font();
    font.setPixelSize(fontSizePixel);
    font.setFamily("KaiTi");
    painter->setFont(font);
    painter->setPen(color);
    auto fm = painter->fontMetrics();
    auto rect = fm.boundingRect(character);
    painter->drawStaticText(-rect.width() / 2.0,
                            -radius_outer + height * 1.0 / 5.0,
                            QStaticText(character));
    painter->restore();
  };

  drawer(angle_8_1 * 0.0, tr("Li"), QColor(0x48D1CC));
  drawer(angle_8_1 * 1.0, tr("Kun"), QColor(0x00FF7F));
  drawer(angle_8_1 * 2.0, tr("Dui"), QColor(0x00BFFF));
  drawer(angle_8_1 * 3.0, tr("Qian"), QColor(0x4682B4));
  drawer(angle_8_1 * 4.0, tr("Kan"), QColor(0x000080));
  drawer(angle_8_1 * 5.0, tr("Gen"), QColor(0xFFA500));
  drawer(angle_8_1 * 6.0, tr("Zhen"), QColor(0xF4A460));
  drawer(angle_8_1 * 7.0, tr("Xun"), QColor(0xFF6347));
}

void BaguaRound::drawCircle(QPainter *painter, qreal radius,
                            const QColor &color, qreal penWidth) {
  painter->save();
  painter->setPen(QPen(QBrush(color), penWidth));
  painter->drawEllipse(QRectF(-radius, -radius, 2.0 * radius, 2.0 * radius));
  painter->restore();
}

void BaguaRound::draw24SolarTerms(QPainter *painter, qreal radius_outer,
                                  qreal radius_inner) {
  painter->save();
  painter->rotate(r_d_->_24_soloar_degree_);

  auto height = radius_outer - radius_inner;
  auto fontSizePixel = height * 2.0 / 4.0;

  auto drawer = [=](qreal degree, const QString &character) {
    painter->save();
    painter->rotate(degree);
    auto font = painter->font();
    font.setPixelSize(fontSizePixel);
    font.setFamily("Micosoft YaHei");
    painter->setFont(font);
    auto fm = painter->fontMetrics();
    auto rect = fm.boundingRect(character);
    painter->drawStaticText(-rect.width() / 2.0,
                            -radius_outer + height * 1.0 / 4.0,
                            QStaticText(character));
    painter->restore();
  };

  drawer(angle_24_1 * 0.0, tr("XiaZhi"));
  drawer(angle_24_1 * 1.0, tr("XiaoShu"));
  drawer(angle_24_1 * 2.0, tr("DaShu"));
  drawer(angle_24_1 * 3.0, tr("LiQiu"));
  drawer(angle_24_1 * 4.0, tr("ChuShu"));
  drawer(angle_24_1 * 5.0, tr("BaiLu"));
  drawer(angle_24_1 * 6.0, tr("QiuFen"));
  drawer(angle_24_1 * 7.0, tr("HanLu"));
  drawer(angle_24_1 * 8.0, tr("ShuangJiang"));
  drawer(angle_24_1 * 9.0, tr("LiDong"));
  drawer(angle_24_1 * 10.0, tr("XiaoXue"));
  drawer(angle_24_1 * 11.0, tr("DaXue"));
  drawer(angle_24_1 * 12.0, tr("DongZhi"));
  drawer(angle_24_1 * 13.0, tr("XiaoHan"));
  drawer(angle_24_1 * 14.0, tr("DaHan"));
  drawer(angle_24_1 * 15.0, tr("LiChun"));
  drawer(angle_24_1 * 16.0, tr("YuShui"));
  drawer(angle_24_1 * 17.0, tr("JingZhe"));
  drawer(angle_24_1 * 18.0, tr("ChunFen"));
  drawer(angle_24_1 * 19.0, tr("QingMing"));
  drawer(angle_24_1 * 20.0, tr("GuYu"));
  drawer(angle_24_1 * 21.0, tr("LiXia"));
  drawer(angle_24_1 * 22.0, tr("XiaoMan"));
  drawer(angle_24_1 * 23.0, tr("MangZhong"));

  painter->restore();
}

void BaguaRound::drawScale(QPainter *painter, qreal radius_outer,
                           qreal radius_inner) {
  static const auto _10Degree = 360 / 36.0;
  auto longerLength = (radius_outer - radius_inner) / 2.0;
  auto shorterLength = longerLength / 2.0;
  auto yOffset = shorterLength / 2.0;

  painter->save();
  QPainterPath path;
  path.addEllipse(-radius_outer, -radius_outer, 2 * radius_outer,
                  2 * radius_outer);
  path.addEllipse(-radius_inner, -radius_inner, 2 * radius_inner,
                  2 * radius_inner);
  painter->fillPath(path, QColor(139, 118, 42));
  painter->restore();

  for (int i = 0; i < 36; ++i) {
    painter->save();
    painter->rotate(_10Degree * i);
    painter->setPen(Qt::blue);
    painter->drawLine(0.0, -radius_outer + yOffset, 0.0,
                      -radius_outer + longerLength + yOffset);
    painter->restore();

    painter->save();
    painter->rotate(_10Degree * i);
    painter->setPen(Qt::white);

    auto str = QString::number(i * 10);
    if (i == 0) {
      str = QString::number(36 * 10);
    }
    auto rect = painter->fontMetrics().boundingRect(str);
    rect.moveCenter(QPoint(0, -radius_outer + longerLength + shorterLength));
    painter->drawText(rect, 0, str);

    painter->restore();
  }

  for (int i = 0; i < 360; ++i) {
    if (i % 10 == 0) continue;
    painter->save();
    painter->rotate(i);
    painter->setPen(Qt::white);
    painter->drawLine(0.0, -radius_outer + yOffset, 0.0,
                      -radius_outer + shorterLength + yOffset);
    painter->restore();
  }
}

void BaguaRound::drawTianGanDiZhi(QPainter *painter, qreal radius_outer,
                                  qreal radius_inner) {
  auto height = radius_outer - radius_inner;
  auto fontSizePixel = height * 2.0 / 4.0;

  auto drawer = [=](qreal degree, const QString &character,
                    const QColor &fontColor = QColor()) {
    painter->save();
    painter->rotate(degree);
    auto font = painter->font();
    font.setPixelSize(fontSizePixel);
    //    font.setFamily("LiSu");
    painter->setFont(font);
    painter->setPen(QPen(fontColor));
    auto fm = painter->fontMetrics();
    auto rect = fm.boundingRect(character);
    painter->drawStaticText(-rect.width() / 2.0,
                            -radius_outer + height * 1.0 / 4.0,
                            QStaticText(character));
    painter->restore();
  };

  drawer(angle_24_1 * 0.0, tr("Wu"), QColor(255, 182, 193));
  drawer(angle_24_1 * 1.0, tr("Ding"), QColor(255, 192, 203));
  drawer(angle_24_1 * 2.0, tr("Wei"), QColor(220, 20, 60));
  drawer(angle_24_1 * 3.0, tr("Kun"), QColor(0, 255, 127));
  drawer(angle_24_1 * 4.0, tr("Shen"), QColor(219, 112, 147));
  drawer(angle_24_1 * 5.0, tr("Geng"), QColor(255, 105, 180));
  drawer(angle_24_1 * 6.0, tr("You"), QColor(255, 20, 147));
  drawer(angle_24_1 * 7.0, tr("Xin"), QColor(199, 21, 133));
  drawer(angle_24_1 * 8.0, tr("Xu"), QColor(139, 0, 139));
  drawer(angle_24_1 * 9.0, tr("Qian"), QColor(70, 130, 180));
  drawer(angle_24_1 * 10.0, tr("Hai"), QColor(135, 206, 235));
  drawer(angle_24_1 * 11.0, tr("Ren"), QColor(173, 216, 230));
  drawer(angle_24_1 * 12.0, tr("Zi"), QColor(176, 224, 230));
  drawer(angle_24_1 * 13.0, tr("Gui"), QColor(95, 158, 160));
  drawer(angle_24_1 * 14.0, tr("Chou"), QColor(0, 255, 255));
  drawer(angle_24_1 * 15.0, tr("Gen"), QColor(0, 128, 128));
  drawer(angle_24_1 * 16.0, tr("Yin"), QColor(0, 206, 209));
  drawer(angle_24_1 * 17.0, tr("Jia"), QColor(72, 209, 204));
  drawer(angle_24_1 * 18.0, tr("Mao"), QColor(127, 255, 0));
  drawer(angle_24_1 * 19.0, tr("Yi"), QColor(255, 140, 0));
  drawer(angle_24_1 * 20.0, tr("Chen"), QColor(255, 99, 71));
  drawer(angle_24_1 * 21.0, tr("Xun"), QColor(0, 255, 0));
  drawer(angle_24_1 * 22.0, tr("Si"), QColor(240, 230, 140));
  drawer(angle_24_1 * 23.0, tr("Bing"), QColor(127, 255, 0));
}

void BaguaRound::drawDiZhiWith5Elements(QPainter *painter, qreal radius_outer,
                                        qreal radius_inner) {
  auto height = radius_outer - radius_inner;
  auto fontSizePixel = height * 2.0 / 4.0;

  auto drawer = [=](qreal degree, const QString &character,
                    const QColor &fontColor = QColor()) {
    painter->save();
    painter->rotate(degree);
    auto font = painter->font();
    font.setPixelSize(fontSizePixel);
    //    font.setFamily("LiSu");
    painter->setFont(font);
    painter->setPen(QPen(fontColor));
    auto fm = painter->fontMetrics();
    auto rect = fm.boundingRect(character);
    painter->drawStaticText(-rect.width() / 2.0,
                            -radius_outer + height * 1.0 / 4.0,
                            QStaticText(character));
    painter->restore();
  };

  drawer(angle_8_1 * 0.0, tr("WuHuo"));
  drawer(angle_8_1 * 0.5 + angle_32_1, tr("WeiTu"));
  drawer(angle_8_1 * 1.0 + angle_32_1, tr("ShenJin"));
  drawer(angle_8_1 * 2.0, tr("YouJin"));
  drawer(angle_8_1 * 2.5 + angle_32_1, tr("XuTu"));
  drawer(angle_8_1 * 3.0 + angle_32_1, tr("HaiShui"));
  drawer(angle_8_1 * 4.0, tr("ZiShui"));
  drawer(angle_8_1 * 4.5 + angle_32_1, tr("ChouTu"));
  drawer(angle_8_1 * 5.0 + angle_32_1, tr("YinMu"));
  drawer(angle_8_1 * 6.0, tr("MaoMu"));
  drawer(angle_8_1 * 6.5 + angle_32_1, tr("ChenTu"));
  drawer(angle_8_1 * 7.0 + angle_32_1, tr("SiShui"));
}

void BaguaRound::drawTianGanWith5Elements(QPainter *painter, qreal radius_outer,
                                          qreal radius_inner) {
  auto height = radius_outer - radius_inner;
  auto fontSizePixel = height * 2.0 / 4.0;

  auto drawer = [=](qreal degree, const QString &character,
                    const QColor &fontColor = QColor()) {
    painter->save();
    painter->rotate(degree);
    auto font = painter->font();
    font.setPixelSize(fontSizePixel);
    //    font.setFamily("LiSu");
    painter->setFont(font);
    painter->setPen(QPen(fontColor));
    auto fm = painter->fontMetrics();
    auto rect = fm.boundingRect(character);
    painter->drawStaticText(-rect.width() / 2.0,
                            -radius_outer + height * 1.0 / 4.0,
                            QStaticText(character));
    painter->restore();
  };

  drawer(angle_8_1 * 0.0, tr("2&7BingDingHuo"));
  drawer(angle_8_1 * 1.0, tr("10YiKunTu"));
  drawer(angle_8_1 * 2.0, tr("4&9GengXinJin"));
  drawer(angle_8_1 * 3.0, tr("QianYangJin"));
  drawer(angle_8_1 * 4.0, tr("1&6RenGuiShui"));
  drawer(angle_8_1 * 5.0, tr("5WuGenTu"));
  drawer(angle_8_1 * 6.0, tr("3&8JiaYiMu"));
  drawer(angle_8_1 * 7.0, tr("XunFengMu"));
}

void BaguaRound::drawOneEighthSeperator(QPainter *painter, qreal radius1,
                                        qreal radius2) {
  static const auto PI = std::acos(-1);
  static auto angle = 2 * PI / 8.0 / 2.0;  // 弧度
  static auto absSinAngle = std::abs(std::sin(angle));
  static auto absCosAngle = std::abs(std::cos(angle));

  auto a_1 = absSinAngle * radius1;
  auto b_1 = absCosAngle * radius1;
  auto a_2 = absSinAngle * radius2;
  auto b_2 = absCosAngle * radius2;

  QVector<QLineF> lines;
  lines << QLineF(QPointF(-a_1, -b_1), QPointF(-a_2, -b_2))
        << QLineF(QPointF(a_1, -b_1), QPointF(a_2, -b_2))
        << QLineF(QPointF(b_1, -a_1), QPointF(b_2, -a_2))
        << QLineF(QPointF(b_1, a_1), QPointF(b_2, a_2))
        << QLineF(QPointF(a_1, b_1), QPointF(a_2, b_2))
        << QLineF(QPointF(-a_1, b_1), QPointF(-a_2, b_2))
        << QLineF(QPointF(-b_1, a_1), QPointF(-b_2, a_2))
        << QLineF(QPointF(-b_1, -a_1), QPointF(-b_2, -a_2));

  painter->save();
  painter->setPen(QPen(QBrush(QColor(0x2F4F4F)), 1.0, Qt::SolidLine,
                       Qt::RoundCap, Qt::MiterJoin));
  painter->drawLines(lines);
  painter->restore();
}

void BaguaRound::drawOneTwentyFourthSeperator(QPainter *painter, qreal radius1,
                                              qreal radius2) {
  static const auto PI = std::acos(-1);
  static auto angle = 2 * PI / 24.0 / 2.0;  // 弧度
  static auto absSinAngle = std::abs(std::sin(angle));
  static auto absCosAngle = std::abs(std::cos(angle));

  auto a_1 = absSinAngle * radius1;
  auto b_1 = absCosAngle * radius1;
  auto a_2 = absSinAngle * radius2;
  auto b_2 = absCosAngle * radius2;

  QVector<QLineF> lines;
  lines << QLineF(QPointF(-a_1, -b_1), QPointF(-a_2, -b_2))
        << QLineF(QPointF(a_1, -b_1), QPointF(a_2, -b_2))
        << QLineF(QPointF(b_1, -a_1), QPointF(b_2, -a_2))
        << QLineF(QPointF(b_1, a_1), QPointF(b_2, a_2))
        << QLineF(QPointF(a_1, b_1), QPointF(a_2, b_2))
        << QLineF(QPointF(-a_1, b_1), QPointF(-a_2, b_2))
        << QLineF(QPointF(-b_1, a_1), QPointF(-b_2, a_2))
        << QLineF(QPointF(-b_1, -a_1), QPointF(-b_2, -a_2));

  painter->save();
  painter->setPen(QPen(QBrush(QColor(Qt::black)), 1.0, Qt::SolidLine,
                       Qt::RoundCap, Qt::MiterJoin));
  painter->drawLines(lines);
  painter->restore();

  painter->save();
  painter->rotate(angle_8_1);
  painter->setPen(QPen(QBrush(QColor(Qt::black)), 1.0, Qt::SolidLine,
                       Qt::RoundCap, Qt::MiterJoin));
  painter->drawLines(lines);
  painter->restore();
}

void BaguaRound::drawQuarterSeperator(QPainter *painter, qreal radius1,
                                      qreal radius2) {
  static const auto PI = std::acos(-1);
  static auto angle = 2 * PI / 4.0 / 2.0;  // 弧度
  static auto absSinAngle = std::abs(std::sin(angle));
  static auto absCosAngle = std::abs(std::cos(angle));

  auto a_1 = absSinAngle * radius1;
  auto b_1 = absCosAngle * radius1;
  auto a_2 = absSinAngle * radius2;
  auto b_2 = absCosAngle * radius2;

  QVector<QLineF> lines;
  lines << QLineF(QPointF(-a_1, -b_1), QPointF(-a_2, -b_2))
        << QLineF(QPointF(a_1, -b_1), QPointF(a_2, -b_2))
        << QLineF(QPointF(a_1, b_1), QPointF(a_2, b_2))
        << QLineF(QPointF(-a_1, b_1), QPointF(-a_2, b_2));

  painter->save();
  painter->setPen(QPen(QBrush(QColor(Qt::red)), 1.0, Qt::SolidLine,
                       Qt::RoundCap, Qt::MiterJoin));
  painter->drawLines(lines);
  painter->restore();
}
