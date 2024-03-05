#include "bagua.h"

#include <QPainter>
#include <QPainterPath>
#include <QTimer>
#include <QtDebug>
#include <cmath>
#include <string>

static auto OneEightCircleDegree = 360.0 / 8.0;

static const char *Li_Unicode = "\347\246\273";
static const char *Kun_Unicode = "\345\235\244";
static const char *Dui_Unicode = "\345\205\221";
static const char *Qian_Unicode = "\344\271\276";
static const char *Kan_Unicode = "\345\235\216";
static const char *Gen_Unicode = "\350\211\256";
static const char *Zhen_Unicode = "\351\234\207";
static const char *Xun_Unicode = "\345\267\275";
static const char *Bing_Unicode = "\344\270\231";
static const char *Ding_Unicode = "\344\270\201";
static const char *Wu_Unicode = "\345\215\210";
static const char *Ma_Unicode = "\351\251\254";
static const char *Wei_Unicode = "\346\234\252";
static const char *Yang_Unicode = "\347\276\212";
static const char *Shen_Unicode = "\347\224\263";
static const char *Hou_Unicode = "\347\214\264";
static const char *Geng_Unicode = "\345\272\232";
static const char *You_Unicode = "\351\205\211";
static const char *Ji_Unicode = "\351\270\241";
static const char *Xin_Unicode = "\350\276\233";
static const char *Xu_Unicode = "\346\210\214";
static const char *Gou_Unicode = "\347\213\227";
static const char *Hai_Unicode = "\344\272\245";
static const char *Zhu_Unicode = "\347\214\252";
static const char *Ren_Unicode = "\345\243\254";
static const char *Zi_Unicode = "\345\255\220";
static const char *Shu_Unicode = "\351\274\240";
static const char *Gui_Unicode = "\347\231\270";
static const char *Chou_Unicode = "\344\270\221";
static const char *Niu_Unicode = "\347\211\233";
static const char *Yin_Unicode = "\345\257\205";
static const char *Hu_Unicode = "\350\231\216";
static const char *Jia_Unicode = "\347\224\262";
static const char *Yi_Unicode = "\344\271\231";
static const char *Mao_Unicode = "\345\215\257";
static const char *Tu_Unicode = "\345\205\224";
static const char *Chen_Unicode = "\350\276\260";
static const char *Long_Unicode = "\351\276\215";
static const char *Si_Unicode = "\345\267\263";
static const char *She_Unicode = "\350\233\207";
static const char *Huo_Unicode = "\347\201\253";
static const char *Nan_Unicode = "\345\215\227";
static const char *Di_Unicode = "\345\234\260";
static const char *XiNan_Unicode = "\350\245\277\345\215\227";
static const char *Ze_Unicode = "\346\263\275";
static const char *Xi_Unicode = "\350\245\277";
static const char *Tian_Unicode = "\345\244\251";
static const char *Shui_Unicode = "\346\260\264";
static const char *Bei_Unicode = "\345\214\227";
static const char *Shan_Unicode = "\345\261\261";
static const char *DongBei_Unicode = "\346\235\261\345\214\227";
static const char *Lei_Unicode = "\351\235\201";
static const char *Dong_Unicode = "\346\235\261";
static const char *Feng_Unicode = "\351\243\216";
static const char *DongNan_Unicode = "\346\235\261\345\215\227";

Bagua::Bagua(QWidget *parent) : QWidget(parent) {
  setMinimumSize(400, 400);

  timer_1 = new QTimer(this);
  connect(timer_1, &QTimer::timeout, this, &Bagua::onTimer1Trick);
  //    timer_1->start(1000/30);
}

void Bagua::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);
  QPainter painter(this);

  painter.fillRect(rect(), Qt::white);
  painter.setRenderHint(QPainter::Antialiasing);
  // Translates the coordinate system to the center
  painter.translate(QPointF(width() / 2.0f, height() / 2.0f));

  drawFishCircle(&painter);
  drawFramesAndCharacters(&painter);
}

void Bagua::drawFishCircle(QPainter *painter) {
  auto fishCircleDiameter = std::min(width(), height()) / 5.0f;
  auto fishCircleRadius = fishCircleDiameter / 2.0f;
  painter->save();
  painter->setPen(Qt::NoPen);
  painter->setBrush(Qt::black);
  painter->drawEllipse(QRectF(-fishCircleRadius, -fishCircleRadius,
                              fishCircleDiameter, fishCircleDiameter));
  painter->restore();

  // white fish head
  auto fishHeadRadius = fishCircleRadius / 2.0f;
  QPainterPath path(QPointF(0.0f, -fishCircleRadius));
  path.arcTo(QRectF(-fishCircleRadius, -fishCircleRadius, fishCircleDiameter,
                    fishCircleDiameter),
             90.0, 180.0);
  path.arcTo(QRectF(-fishHeadRadius, 0.0, fishCircleRadius, fishCircleRadius),
             -90.0, -180.0);
  path.arcTo(QRectF(-fishHeadRadius, -fishCircleRadius, fishCircleRadius,
                    fishCircleRadius),
             -90.0, 180.0);
  painter->save();
  painter->setPen(Qt::NoPen);
  painter->fillPath(path, Qt::white);
  painter->restore();

  // white fish eye
  auto fishEyeWidth = fishCircleRadius / 2.5f;
  painter->save();
  painter->setPen(QPen(QBrush(Qt::black), fishEyeWidth, Qt::SolidLine,
                       Qt::RoundCap, Qt::RoundJoin));
  painter->drawPoint(QPointF(0.0f, -fishCircleRadius / 2.0f));
  painter->restore();

  // black fish eye
  painter->save();
  painter->setPen(QPen(QBrush(Qt::white), fishEyeWidth, Qt::SolidLine,
                       Qt::RoundCap, Qt::RoundJoin));
  painter->drawPoint(QPointF(0.0f, fishCircleRadius / 2.0f));
  painter->restore();
}

void Bagua::drawFramesAndCharacters(QPainter *painter) {
  painter->save();
  painter->rotate(degree1_);

  static const auto PI = std::acos(-1);

  /* 以梯形较长的底到坐标系的原点（坐标系原点已经移动到）做垂直线,形成一个直角三角形
   * A——顶点
   * D——顶点
   * B——顶点
   * O——坐标原点
   * a——边长 = AD
   * b——边长 = OD
   * β（贝塔）——∠bc（角bc）
   *A ___a___D_______B
   *  \      |      /
   *   \     |     /
   *    \   b|    /
   *   c \   |   /
   *      \  |  /
   *       \β| /
   *        \|/
   *         O——————————————————————> x
   *         |
   *         |
   *         |
   *         |
   *         |
   *       y ↓
   */

  // cosβ = b / c

  auto width = std::min(this->width(), height());
  auto radius = width / 2.0;
  auto angleRadian = 2 * PI / 8.0 / 2.0;  // 2Π 弧度 = 360 °

  auto absSinAngle = std::abs(
      std::sin(angleRadian));  // double std::sin(double x)中x用的是弧度
  auto absCosAngle = std::abs(std::cos(angleRadian));

  auto radius_outer_to_inner_1 = radius * 290.0 / 300.0;   // 最外层框线
  auto radius_outer_to_inner_2 = radius * 270.0 / 300.0;   // 方位和元素
  auto radius_outer_to_inner_3 = radius * 220.0 / 300.0;   // 第二层框线
  auto radius_outer_to_inner_4 = radius * 200.0 / 300.0;   // 第三层框线
  auto radius_outer_to_inner_5 = radius * 190.0 / 300.0;   // 天干地支
  auto radius_outer_to_inner_6 = radius * 160.0 / 300.0;   // 生肖
  auto radius_outer_to_inner_7 = radius * 140.0 / 300.0;   // 第四层框线
  auto radius_outer_to_inner_8 = radius * 135.0 / 300.0;   // 卦汉字
  auto radius_outer_to_inner_9 = radius * 100.0 / 300.0;   // 第五层框线
  auto radius_outer_to_inner_10 = radius * 88.00 / 300.0;  // 卦符

  // 外边的一半
  auto a_1 = absSinAngle * radius_outer_to_inner_1;
  // 外边到圆心垂直线的长度
  auto b_1 = absCosAngle * radius_outer_to_inner_1;

  auto a_2 = absSinAngle * radius_outer_to_inner_2;
  auto b_2 = absCosAngle * radius_outer_to_inner_2;

  auto a_3 = absSinAngle * radius_outer_to_inner_3;
  auto b_3 = absCosAngle * radius_outer_to_inner_3;

  auto a_4 = absSinAngle * radius_outer_to_inner_4;
  auto b_4 = absCosAngle * radius_outer_to_inner_4;

  auto a_5 = absSinAngle * radius_outer_to_inner_5;
  auto b_5 = absCosAngle * radius_outer_to_inner_5;

  auto a_6 = absSinAngle * radius_outer_to_inner_6;
  auto b_6 = absCosAngle * radius_outer_to_inner_6;

  auto a_7 = absSinAngle * radius_outer_to_inner_7;
  auto b_7 = absCosAngle * radius_outer_to_inner_7;

  auto a_8 = absSinAngle * radius_outer_to_inner_8;
  auto b_8 = absCosAngle * radius_outer_to_inner_8;

  auto a_9 = absSinAngle * radius_outer_to_inner_9;
  auto b_9 = absCosAngle * radius_outer_to_inner_9;

  auto a_10 = absSinAngle * radius_outer_to_inner_10;
  auto b_10 = absCosAngle * radius_outer_to_inner_10;

  // 多层八边形
  drawOctagon(painter, width * 3.0 / 400.0, color_frame_1_, a_1, b_1);
  drawOctagon(painter, width * 4.0 / 400.0, color_frame_2_, a_3, b_3);
  drawOctagon(painter, width * 2.0 / 400.0, color_frame_3_, a_4, b_4);
  drawOctagon(painter, width * 4.0 / 400.0, color_frame_4_, a_7, b_7);
  drawOctagon(painter, width * 2.0 / 400.0, color_frame_5_, a_9, b_9);

  // 块分割线
  drawSpacer(painter, width * 2.5 / 400.0, color_spacer_1_,
             QLineF(QPointF(-a_1, -b_1), QPointF(-a_9, -b_9)));
  drawSpacer(painter, width * 2.5 / 400.0, color_spacer_2_,
             QLineF(QPointF(a_1, -b_1), QPointF(a_9, -b_9)));
  drawSpacer(painter, width * 2.5 / 400.0, color_spacer_3_,
             QLineF(QPointF(b_1, -a_1), QPointF(b_9, -a_9)));
  drawSpacer(painter, width * 2.5 / 400.0, color_spacer_4_,
             QLineF(QPointF(b_1, a_1), QPointF(b_9, a_9)));
  drawSpacer(painter, width * 2.5 / 400.0, color_spacer_5_,
             QLineF(QPointF(a_1, b_1), QPointF(a_9, b_9)));
  drawSpacer(painter, width * 2.5 / 400.0, color_spacer_6_,
             QLineF(QPointF(-a_1, b_1), QPointF(-a_9, b_9)));
  drawSpacer(painter, width * 2.5 / 400.0, color_spacer_7_,
             QLineF(QPointF(-b_1, a_1), QPointF(-b_9, a_9)));
  drawSpacer(painter, width * 2.5 / 400.0, color_spacer_8_,
             QLineF(QPointF(-b_1, -a_1), QPointF(-b_9, -a_9)));

  // 垂直线
  auto penWidth = width * 2.0 / 400.0;
  for (auto i = 0; i < 8; i++) {
    painter->save();
    painter->rotate(OneEightCircleDegree * i);
    painter->setPen(QPen(QBrush(0x000001), penWidth));
    painter->drawLine(QPointF(-a_4 / 3.0, -b_4), QPointF(-a_4 / 3.0, -b_7));
    painter->drawLine(QPointF(a_4 / 3.0, -b_4), QPointF(a_4 / 3.0, -b_7));
    painter->restore();
  }

  // 画卦
  drawGua(painter, a_10, b_10);

  // 画卦文字
  drawGuaCharacter(painter, QPointF(a_7, b_7), QPointF(a_8, b_8),
                   QPointF(a_9, b_9));

  // 画天干地支
  drawTianGanAndDiZhi(painter, QPointF(a_4, b_4), QPointF(a_5, b_5),
                      QPointF(a_6, b_6), QPointF(a_7, b_7));

  // 画最外层的方位和元素
  drawDirectionAndElement(painter, QPointF(a_1, b_1), QPointF(a_2, b_2),
                          QPointF(a_3, b_3));

  painter->restore();
}

void Bagua::drawGua(QPainter *painter, qreal a, qreal b) {
  auto penWidth = std::min(this->width(), height()) * 2.0 / 400.0;

  QVector<QLineF> lines;

  // 离中虚
  lines.clear();
  painter->save();
  painter->rotate(OneEightCircleDegree * 0.0);
  painter->setPen(QPen(color_gua_li_, penWidth));

  lines.append(QLineF(QPointF(-a / 2.0f, -b), QPointF(a / 2.0f, -b)));

  lines.append(QLineF(QPointF(-a / 2.0f, penWidth * 2 - b),
                      QPointF(-penWidth, penWidth * 2 - b)));
  lines.append(QLineF(QPointF(penWidth, penWidth * 2 - b),
                      QPointF(a / 2.0f, penWidth * 2 - b)));

  lines.append(QLineF(QPointF(-a / 2.0f, penWidth * 4 - b),
                      QPointF(a / 2.0f, penWidth * 4 - b)));
  painter->drawLines(lines);
  painter->restore();

  // 坤六断
  lines.clear();
  painter->save();
  painter->rotate(OneEightCircleDegree * 1);
  painter->setPen(QPen(color_gua_kun_, penWidth));
  lines.append(QLineF(QPointF(-a / 2.0f, -b), QPointF(-penWidth, -b)));
  lines.append(QLineF(QPointF(penWidth, -b), QPointF(a / 2.0f, -b)));

  lines.append(QLineF(QPointF(-a / 2.0f, penWidth * 2 - b),
                      QPointF(-penWidth, penWidth * 2 - b)));
  lines.append(QLineF(QPointF(penWidth, penWidth * 2 - b),
                      QPointF(a / 2.0f, penWidth * 2 - b)));

  lines.append(QLineF(QPointF(-a / 2.0f, penWidth * 4 - b),
                      QPointF(-penWidth, penWidth * 4 - b)));
  lines.append(QLineF(QPointF(penWidth, penWidth * 4 - b),
                      QPointF(a / 2.0f, penWidth * 4 - b)));
  painter->drawLines(lines);
  painter->restore();

  // 兑上缺
  lines.clear();
  painter->save();
  painter->rotate(OneEightCircleDegree * 2);
  painter->setPen(QPen(color_gua_dui_, penWidth));
  lines.append(QLineF(QPointF(-a / 2.0f, -b), QPointF(-penWidth, -b)));
  lines.append(QLineF(QPointF(penWidth, -b), QPointF(a / 2.0f, -b)));

  lines.append(QLineF(QPointF(-a / 2.0f, penWidth * 2 - b),
                      QPointF(a / 2.0f, penWidth * 2 - b)));

  lines.append(QLineF(QPointF(-a / 2.0f, penWidth * 4 - b),
                      QPointF(a / 2.0f, penWidth * 4 - b)));
  painter->drawLines(lines);
  painter->restore();

  // 乾三连
  lines.clear();
  painter->save();
  painter->setPen(QPen(color_gua_qian_, penWidth));
  painter->rotate(OneEightCircleDegree * 3);
  lines.append(QLineF(QPointF(-a / 2.0f, -b), QPointF(a / 2.0f, -b)));

  lines.append(QLineF(QPointF(-a / 2.0f, penWidth * 2 - b),
                      QPointF(a / 2.0f, penWidth * 2 - b)));

  lines.append(QLineF(QPointF(-a / 2.0f, penWidth * 4 - b),
                      QPointF(a / 2.0f, penWidth * 4 - b)));
  painter->drawLines(lines);
  painter->restore();

  // 坎中满
  lines.clear();
  painter->save();
  painter->setPen(QPen(color_gua_kan_, penWidth));
  painter->rotate(OneEightCircleDegree * 4);
  lines.append(QLineF(QPointF(-a / 2.0f, -b), QPointF(-penWidth, -b)));
  lines.append(QLineF(QPointF(penWidth, -b), QPointF(a / 2.0f, -b)));

  lines.append(QLineF(QPointF(-a / 2.0f, penWidth * 2 - b),
                      QPointF(a / 2.0f, penWidth * 2 - b)));

  lines.append(QLineF(QPointF(-a / 2.0f, penWidth * 4 - b),
                      QPointF(-penWidth, penWidth * 4 - b)));
  lines.append(QLineF(QPointF(penWidth, penWidth * 4 - b),
                      QPointF(a / 2.0f, penWidth * 4 - b)));
  painter->drawLines(lines);
  painter->restore();

  // 艮覆碗
  lines.clear();
  painter->save();
  painter->setPen(QPen(color_gua_gen_, penWidth));
  painter->rotate(OneEightCircleDegree * 5);
  lines.append(QLineF(QPointF(-a / 2.0f, -b), QPointF(a / 2.0f, -b)));

  lines.append(QLineF(QPointF(-a / 2.0f, penWidth * 2 - b),
                      QPointF(-penWidth, penWidth * 2 - b)));
  lines.append(QLineF(QPointF(penWidth, penWidth * 2 - b),
                      QPointF(a / 2.0f, penWidth * 2 - b)));

  lines.append(QLineF(QPointF(-a / 2.0f, penWidth * 4 - b),
                      QPointF(-penWidth, penWidth * 4 - b)));
  lines.append(QLineF(QPointF(penWidth, penWidth * 4 - b),
                      QPointF(a / 2.0f, penWidth * 4 - b)));
  painter->drawLines(lines);
  painter->restore();

  // 震仰盂
  lines.clear();
  painter->save();
  painter->setPen(QPen(color_gua_zhen_, penWidth));
  painter->rotate(OneEightCircleDegree * 6);
  lines.append(QLineF(QPointF(-a / 2.0f, -b), QPointF(-penWidth, -b)));
  lines.append(QLineF(QPointF(penWidth, -b), QPointF(a / 2.0f, -b)));

  lines.append(QLineF(QPointF(-a / 2.0f, penWidth * 2 - b),
                      QPointF(-penWidth, penWidth * 2 - b)));
  lines.append(QLineF(QPointF(penWidth, penWidth * 2 - b),
                      QPointF(a / 2.0f, penWidth * 2 - b)));

  lines.append(QLineF(QPointF(-a / 2.0f, penWidth * 4 - b),
                      QPointF(a / 2.0f, penWidth * 4 - b)));
  painter->drawLines(lines);
  painter->restore();

  // 巽下断
  lines.clear();
  painter->save();
  painter->setPen(QPen(color_gua_xun_, penWidth));
  painter->rotate(OneEightCircleDegree * 7);
  lines.append(QLineF(QPointF(-a / 2.0f, -b), QPointF(a / 2.0f, -b)));

  lines.append(QLineF(QPointF(-a / 2.0f, penWidth * 2 - b),
                      QPointF(a / 2.0f, penWidth * 2 - b)));

  lines.append(QLineF(QPointF(-a / 2.0f, penWidth * 4 - b),
                      QPointF(-penWidth, penWidth * 4 - b)));
  lines.append(QLineF(QPointF(penWidth, penWidth * 4 - b),
                      QPointF(a / 2.0f, penWidth * 4 - b)));
  painter->drawLines(lines);
  painter->restore();
}

void Bagua::drawOctagon(QPainter *painter, qreal penWidth, const QColor &color,
                        qreal a, qreal b) {
  QPainterPath path(QPointF(-a, -b));
  path.lineTo(QPointF(a, -b));
  path.lineTo(QPointF(b, -a));
  path.lineTo(QPointF(b, a));
  path.lineTo(QPointF(a, b));
  path.lineTo(QPointF(-a, b));
  path.lineTo(QPointF(-b, a));
  path.lineTo(QPointF(-b, -a));
  path.lineTo(QPointF(-a, -b));

  painter->save();
  painter->setPen(QPen(QBrush(color), penWidth));
  painter->drawPath(path);
  painter->restore();
}

void Bagua::drawSpacer(QPainter *painter, qreal penWidth, const QColor &color,
                       const QLineF &line) {
  painter->save();
  painter->setPen(QPen(QBrush(color), penWidth));
  painter->drawLine(line);
  painter->restore();
}

void Bagua::drawGuaCharacter(QPainter *painter, const QPointF &outer,
                             const QPointF &base, const QPointF &inner) {
  // 计算字体的像素大小
  auto spacing = outer.y() - base.y();
  int fontPixSize = outer.y() - inner.y() - spacing;
  painter->save();
  auto font = painter->font();
  font.setFamily("STKaiti");
  font.setBold(true);
  font.setPixelSize(fontPixSize);
  painter->setFont(font);

  painter->save();
  painter->rotate(OneEightCircleDegree * 0.0);
  auto fm = painter->fontMetrics();
  auto rect = fm.boundingRect(Li_Unicode);
  painter->drawText(QRectF(-rect.width() / 2.0, -base.y() - spacing / 2.0,
                           rect.width(), rect.height()),
                    0, Li_Unicode);
  painter->restore();

  painter->save();
  painter->rotate(OneEightCircleDegree * 1.0);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Kun_Unicode);
  painter->drawText(QRectF(-rect.width() / 2.0, -base.y() - spacing / 2.0,
                           rect.width(), rect.height()),
                    0, Kun_Unicode);
  painter->restore();

  painter->save();
  painter->rotate(OneEightCircleDegree * 2.0);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Dui_Unicode);
  painter->drawText(QRectF(-rect.width() / 2.0, -base.y() - spacing / 2.0,
                           rect.width(), rect.height()),
                    0, Dui_Unicode);
  painter->restore();

  painter->save();
  painter->rotate(OneEightCircleDegree * 3.0);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Qian_Unicode);
  painter->drawText(QRectF(-rect.width() / 2.0, -base.y() - spacing / 2.0,
                           rect.width(), rect.height()),
                    0, Qian_Unicode);
  painter->restore();

  painter->save();
  painter->rotate(OneEightCircleDegree * 4.0);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Kan_Unicode);
  painter->drawText(QRectF(-rect.width() / 2.0, -base.y() - spacing / 2.0,
                           rect.width(), rect.height()),
                    0, Kan_Unicode);
  painter->restore();

  painter->save();
  painter->rotate(OneEightCircleDegree * 5.0);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Gen_Unicode);
  painter->drawText(QRectF(-rect.width() / 2.0, -base.y() - spacing / 2.0,
                           rect.width(), rect.height()),
                    0, Gen_Unicode);
  painter->restore();

  painter->save();
  painter->rotate(OneEightCircleDegree * 6.0);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Zhen_Unicode);
  painter->drawText(QRectF(-rect.width() / 2.0, -base.y() - spacing / 2.0,
                           rect.width(), rect.height()),
                    0, Zhen_Unicode);
  painter->restore();

  painter->save();
  painter->rotate(OneEightCircleDegree * 7.0);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Xun_Unicode);
  painter->drawText(QRectF(-rect.width() / 2.0, -base.y() - spacing / 2.0,
                           rect.width(), rect.height()),
                    0, Xun_Unicode);
  painter->restore();

  painter->restore();
}

void Bagua::drawDirectionAndElement(QPainter *painter, const QPointF &outer,
                                    const QPointF &base, const QPointF &inner) {
  // 计算字体的像素大小
  auto spacing = outer.y() - base.y();
  int fontPixSize = outer.y() - inner.y() - spacing;

  /******
   * 南
   * */
  painter->save();
  painter->rotate(OneEightCircleDegree * 0.0);

  painter->save();
  auto font = painter->font();
  font.setFamily("YouYuan");
  font.setBold(true);
  font.setPixelSize(fontPixSize / 2.0);
  painter->setFont(font);
  auto fm = painter->fontMetrics();
  auto rect = fm.boundingRect(Huo_Unicode);
  painter->drawText(QRectF(-base.x() + rect.width(), -base.y() + spacing / 4.0,
                           rect.width(), rect.height()),
                    0, Huo_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("LiSu");
  font.setBold(true);
  font.setPixelSize(fontPixSize);
  painter->setFont(font);
  painter->setPen(QColor(161, 24, 15));
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Nan_Unicode);
  painter->drawText(QRectF(-rect.width() / 2.0, -base.y() - spacing / 2.0,
                           rect.width(), rect.height()),
                    0, Nan_Unicode);
  painter->restore();

  painter->restore();

  /******
   * 西南
   * */
  painter->save();
  painter->rotate(OneEightCircleDegree * 1.0);

  painter->save();
  font = painter->font();
  font.setFamily("YouYuan");
  font.setBold(true);
  font.setPixelSize(fontPixSize / 2.0);
  painter->setFont(font);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Di_Unicode);
  painter->drawText(QRectF(-base.x() + rect.width(), -base.y() + spacing / 4.0,
                           rect.width(), rect.height()),
                    0, Di_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("LiSu");
  font.setBold(true);
  font.setPixelSize(fontPixSize);
  painter->setFont(font);
  painter->setPen(QColor(161, 24, 15));
  fm = painter->fontMetrics();
  rect = fm.boundingRect(XiNan_Unicode);
  painter->drawText(QRectF(-rect.width() / 2.0, -base.y() - spacing / 2.0,
                           rect.width(), rect.height()),
                    0, XiNan_Unicode);
  painter->restore();

  painter->restore();

  /******
   * 西
   * */
  painter->save();
  painter->rotate(OneEightCircleDegree * 2.0);

  painter->save();
  font = painter->font();
  font.setFamily("YouYuan");
  font.setBold(true);
  font.setPixelSize(fontPixSize / 2.0);
  painter->setFont(font);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Ze_Unicode);
  painter->drawText(QRectF(-base.x() + rect.width(), -base.y() + spacing / 4.0,
                           rect.width(), rect.height()),
                    0, Ze_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("LiSu");
  font.setBold(true);
  font.setPixelSize(fontPixSize);
  painter->setFont(font);
  painter->setPen(QColor(161, 24, 15));
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Xi_Unicode);
  painter->drawText(QRectF(-rect.width() / 2.0, -base.y() - spacing / 2.0,
                           rect.width(), rect.height()),
                    0, Xi_Unicode);
  painter->restore();

  painter->restore();

  /******
   * 西北
   * */
  painter->save();
  painter->rotate(OneEightCircleDegree * 3.0);

  painter->save();
  font = painter->font();
  font.setFamily("YouYuan");
  font.setBold(true);
  font.setPixelSize(fontPixSize / 2.0);
  painter->setFont(font);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Tian_Unicode);
  painter->drawText(QRectF(-base.x() + rect.width(), -base.y() + spacing / 4.0,
                           rect.width(), rect.height()),
                    0, Tian_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("LiSu");
  font.setBold(true);
  font.setPixelSize(fontPixSize);
  painter->setFont(font);
  painter->setPen(QColor(161, 24, 15));
  fm = painter->fontMetrics();
  rect = fm.boundingRect("\350\245\277\345\214\227");
  painter->drawText(QRectF(-rect.width() / 2.0, -base.y() - spacing / 2.0,
                           rect.width(), rect.height()),
                    0, "\350\245\277\345\214\227");
  painter->restore();

  painter->restore();

  /******
   * 北
   * */
  painter->save();
  painter->rotate(OneEightCircleDegree * 4.0);

  painter->save();
  font = painter->font();
  font.setFamily("YouYuan");
  font.setBold(true);
  font.setPixelSize(fontPixSize / 2.0);
  painter->setFont(font);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Shui_Unicode);
  painter->drawText(QRectF(-base.x() + rect.width(), -base.y() + spacing / 4.0,
                           rect.width(), rect.height()),
                    0, Shui_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("LiSu");
  font.setBold(true);
  font.setPixelSize(fontPixSize);
  painter->setFont(font);
  painter->setPen(QColor(161, 24, 15));
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Bei_Unicode);
  painter->drawText(QRectF(-rect.width() / 2.0, -base.y() - spacing / 2.0,
                           rect.width(), rect.height()),
                    0, Bei_Unicode);
  painter->restore();

  painter->restore();

  /******
   * 东北
   * */
  painter->save();
  painter->rotate(OneEightCircleDegree * 5.0);

  painter->save();
  font = painter->font();
  font.setFamily("YouYuan");
  font.setBold(true);
  font.setPixelSize(fontPixSize / 2.0);
  painter->setFont(font);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Shan_Unicode);
  painter->drawText(QRectF(-base.x() + rect.width(), -base.y() + spacing / 4.0,
                           rect.width(), rect.height()),
                    0, Shan_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("LiSu");
  font.setBold(true);
  font.setPixelSize(fontPixSize);
  painter->setFont(font);
  painter->setPen(QColor(161, 24, 15));
  fm = painter->fontMetrics();
  rect = fm.boundingRect(DongBei_Unicode);
  painter->drawText(QRectF(-rect.width() / 2.0, -base.y() - spacing / 2.0,
                           rect.width(), rect.height()),
                    0, DongBei_Unicode);
  painter->restore();

  painter->restore();

  /******
   * 东
   * */
  painter->save();
  painter->rotate(OneEightCircleDegree * 6.0);

  painter->save();
  font = painter->font();
  font.setFamily("YouYuan");
  font.setBold(true);
  font.setPixelSize(fontPixSize / 2.0);
  painter->setFont(font);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Lei_Unicode);
  painter->drawText(QRectF(-base.x() + rect.width(), -base.y() + spacing / 4.0,
                           rect.width(), rect.height()),
                    0, Lei_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("LiSu");
  font.setBold(true);
  font.setPixelSize(fontPixSize);
  painter->setFont(font);
  painter->setPen(QColor(161, 24, 15));
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Dong_Unicode);
  painter->drawText(QRectF(-rect.width() / 2.0, -base.y() - spacing / 2.0,
                           rect.width(), rect.height()),
                    0, Dong_Unicode);
  painter->restore();

  painter->restore();

  /******
   * 东南
   * */
  painter->save();
  painter->rotate(OneEightCircleDegree * 7.0);

  painter->save();
  font = painter->font();
  font.setFamily("YouYuan");
  font.setBold(true);
  font.setPixelSize(fontPixSize / 2.0);
  painter->setFont(font);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Feng_Unicode);
  painter->drawText(QRectF(-base.x() + rect.width(), -base.y() + spacing / 4.0,
                           rect.width(), rect.height()),
                    0, Feng_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("LiSu");
  font.setBold(true);
  font.setPixelSize(fontPixSize);
  painter->setFont(font);
  painter->setPen(QColor(161, 24, 15));
  fm = painter->fontMetrics();
  rect = fm.boundingRect(DongNan_Unicode);
  painter->drawText(QRectF(-rect.width() / 2.0, -base.y() - spacing / 2.0,
                           rect.width(), rect.height()),
                    0, DongNan_Unicode);
  painter->restore();

  painter->restore();
}

void Bagua::drawTianGanAndDiZhi(QPainter *painter, const QPointF &outer,
                                const QPointF &base, const QPointF &inner1,
                                const QPointF &inner2) {
  // 计算字体的像素大小
  auto spacing1 = outer.y() - base.y();
  int fontPixSize1 = outer.y() - inner1.y() - spacing1;

  /******
   * 丙 午马 丁
   * */
  painter->save();
  painter->rotate(OneEightCircleDegree * 0.0);

  painter->save();
  auto font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  font.setPixelSize(fontPixSize1 * 3.0 / 4.0);
  painter->setFont(font);
  auto fm = painter->fontMetrics();
  auto rect = fm.boundingRect(Bing_Unicode);
  painter->drawText(
      QRectF(-base.x() + (base.x() * 2.0 / 3.0 - rect.width()) / 2.0,
             -base.y() + spacing1 / 4.0, rect.width(), rect.height()),
      0, Bing_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  font.setPixelSize(fontPixSize1);
  painter->setFont(font);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Wu_Unicode);
  painter->drawText(QRectF(-rect.width() / 2.0, -base.y() - spacing1 / 2.0,
                           rect.width(), rect.height()),
                    0, Wu_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  auto spacing2 = base.y() - fm.boundingRect(Wu_Unicode).height() - inner1.y();
  auto fontPixSize2 = inner1.y() - inner2.y() - spacing2;
  font.setPixelSize(fontPixSize2);
  painter->setFont(font);
  painter->setPen(QColor());
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Ma_Unicode);
  auto tempR = QRectF(-rect.width() / 2.0, -inner1.y() - rect.height() / 4.0,
                      rect.width(), rect.height());
  QPainterPath path;
  path.addRoundedRect(tempR.marginsAdded(QMarginsF(tempR.width() / 3.0, 0,
                                                   tempR.width() / 3.0, 0)),
                      tempR.width() / 2.0, tempR.height() / 2.0);
  painter->fillPath(path, QColor(161, 24, 15));
  painter->setPen(Qt::white);
  painter->drawText(tempR, 0, Ma_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  font.setPixelSize(fontPixSize1 * 3.0 / 4.0);
  painter->setFont(font);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Ding_Unicode);
  painter->drawText(
      QRectF(base.x() / 3.0 + (base.x() * 2.0 / 3.0 - rect.width()) / 2.0,
             -base.y() + spacing1 / 4.0, rect.width(), rect.height()),
      0, Ding_Unicode);
  painter->restore();

  painter->restore();

  /******
   * 未羊 坤 申猴
   * */
  painter->save();
  painter->rotate(OneEightCircleDegree * 1.0);

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  font.setPixelSize(fontPixSize1);
  painter->setFont(font);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Wei_Unicode);
  painter->drawText(
      QRectF(-base.x() + (base.x() * 2.0 / 3.0 - rect.width()) / 2.0,
             -base.y() - spacing1 / 2.0, rect.width(), rect.height()),
      0, Wei_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  spacing2 = base.y() - fm.boundingRect(Wei_Unicode).height() - inner1.y();
  fontPixSize2 = inner1.y() - inner2.y() - spacing2;
  font.setPixelSize(fontPixSize2);
  painter->setFont(font);
  painter->setPen(QColor());
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Yang_Unicode);
  tempR =
      QRectF(-inner1.x() + (inner1.x() * 2.0 / 3.0 - rect.width()) / 2.0,
             -inner1.y() - rect.height() / 4.0, rect.width(), rect.height());
  QPainterPath path2;
  path2.addRoundedRect(tempR.marginsAdded(QMarginsF(tempR.width() / 3.0, 0,
                                                    tempR.width() / 3.0, 0)),
                       tempR.width() / 2.0, tempR.height() / 2.0);
  painter->fillPath(path2, QColor(161, 24, 15));
  painter->setPen(Qt::white);
  painter->drawText(tempR, 0, Yang_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  font.setPixelSize(fontPixSize1 * 3.0 / 4.0);
  painter->setFont(font);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Kun_Unicode);
  painter->drawText(QRectF(-rect.width() / 2.0, -base.y() - spacing1 / 2.0,
                           rect.width(), rect.height()),
                    0, Kun_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  font.setPixelSize(fontPixSize1);
  painter->setFont(font);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Shen_Unicode);
  painter->drawText(
      QRectF(base.x() / 3.0 + (base.x() * 2.0 / 3.0 - rect.width()) / 2.0,
             -base.y() - spacing1 / 2.0, rect.width(), rect.height()),
      0, Shen_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  spacing2 = base.y() - fm.boundingRect(Shen_Unicode).height() - inner1.y();
  fontPixSize2 = inner1.y() - inner2.y() - spacing2;
  font.setPixelSize(fontPixSize2);
  painter->setFont(font);
  painter->setPen(QColor());
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Hou_Unicode);
  tempR =
      QRectF(inner1.x() / 3.0 + (inner1.x() * 2.0 / 3.0 - rect.width()) / 2.0,
             -inner1.y() - rect.height() / 4.0, rect.width(), rect.height());
  QPainterPath path3;
  path3.addRoundedRect(tempR.marginsAdded(QMarginsF(tempR.width() / 3.0, 0,
                                                    tempR.width() / 3.0, 0)),
                       tempR.width() / 2.0, tempR.height() / 2.0);
  painter->fillPath(path3, QColor(161, 24, 15));
  painter->setPen(Qt::white);
  painter->drawText(tempR, 0, Hou_Unicode);
  painter->restore();

  painter->restore();

  /******
   * 庚 酉鸡 辛
   * */
  painter->save();
  painter->rotate(OneEightCircleDegree * 2.0);

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  font.setPixelSize(fontPixSize1 * 3.0 / 4.0);
  painter->setFont(font);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Geng_Unicode);
  painter->drawText(
      QRectF(-base.x() + (base.x() * 2.0 / 3.0 - rect.width()) / 2.0,
             -base.y() + spacing1 / 4.0, rect.width(), rect.height()),
      0, Geng_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  font.setPixelSize(fontPixSize1);
  painter->setFont(font);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(You_Unicode);
  painter->drawText(QRectF(-rect.width() / 2.0, -base.y() - spacing1 / 2.0,
                           rect.width(), rect.height()),
                    0, You_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  spacing2 = base.y() - fm.boundingRect(You_Unicode).height() - inner1.y();
  fontPixSize2 = inner1.y() - inner2.y() - spacing2;
  font.setPixelSize(fontPixSize2);
  painter->setFont(font);
  painter->setPen(QColor());
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Ji_Unicode);
  tempR = QRectF(-rect.width() / 2.0, -inner1.y() - rect.height() / 4.0,
                 rect.width(), rect.height());
  QPainterPath path4;
  path4.addRoundedRect(tempR.marginsAdded(QMarginsF(tempR.width() / 3.0, 0,
                                                    tempR.width() / 3.0, 0)),
                       tempR.width() / 2.0, tempR.height() / 2.0);
  painter->fillPath(path4, QColor(161, 24, 15));
  painter->setPen(Qt::white);
  painter->drawText(tempR, 0, Ji_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  font.setPixelSize(fontPixSize1 * 3.0 / 4.0);
  painter->setFont(font);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Xin_Unicode);
  painter->drawText(
      QRectF(base.x() / 3.0 + (base.x() * 2.0 / 3.0 - rect.width()) / 2.0,
             -base.y() + spacing1 / 4.0, rect.width(), rect.height()),
      0, Xin_Unicode);
  painter->restore();

  painter->restore();

  /******
   * 戌狗 乾 亥猪
   * */
  painter->save();
  painter->rotate(OneEightCircleDegree * 3.0);

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  font.setPixelSize(fontPixSize1);
  painter->setFont(font);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Xu_Unicode);
  painter->drawText(
      QRectF(-base.x() + (base.x() * 2.0 / 3.0 - rect.width()) / 2.0,
             -base.y() - spacing1 / 2.0, rect.width(), rect.height()),
      0, Xu_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  spacing2 = base.y() - fm.boundingRect(Xu_Unicode).height() - inner1.y();
  fontPixSize2 = inner1.y() - inner2.y() - spacing2;
  font.setPixelSize(fontPixSize2);
  painter->setFont(font);
  painter->setPen(QColor());
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Gou_Unicode);
  tempR =
      QRectF(-inner1.x() + (inner1.x() * 2.0 / 3.0 - rect.width()) / 2.0,
             -inner1.y() - rect.height() / 4.0, rect.width(), rect.height());
  QPainterPath path5;
  path5.addRoundedRect(tempR.marginsAdded(QMarginsF(tempR.width() / 3.0, 0,
                                                    tempR.width() / 3.0, 0)),
                       tempR.width() / 2.0, tempR.height() / 2.0);
  painter->fillPath(path5, QColor(161, 24, 15));
  painter->setPen(Qt::white);
  painter->drawText(tempR, 0, Gou_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  font.setPixelSize(fontPixSize1 * 3.0 / 4.0);
  painter->setFont(font);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Qian_Unicode);
  painter->drawText(QRectF(-rect.width() / 2.0, -base.y() - spacing1 / 2.0,
                           rect.width(), rect.height()),
                    0, Qian_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  font.setPixelSize(fontPixSize1);
  painter->setFont(font);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Hai_Unicode);
  painter->drawText(
      QRectF(base.x() / 3.0 + (base.x() * 2.0 / 3.0 - rect.width()) / 2.0,
             -base.y() - spacing1 / 2.0, rect.width(), rect.height()),
      0, Hai_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  spacing2 = base.y() - fm.boundingRect(Hai_Unicode).height() - inner1.y();
  fontPixSize2 = inner1.y() - inner2.y() - spacing2;
  font.setPixelSize(fontPixSize2);
  painter->setFont(font);
  painter->setPen(QColor());
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Zhu_Unicode);
  tempR =
      QRectF(inner1.x() / 3.0 + (inner1.x() * 2.0 / 3.0 - rect.width()) / 2.0,
             -inner1.y() - rect.height() / 4.0, rect.width(), rect.height());
  QPainterPath path6;
  path6.addRoundedRect(tempR.marginsAdded(QMarginsF(tempR.width() / 3.0, 0,
                                                    tempR.width() / 3.0, 0)),
                       tempR.width() / 2.0, tempR.height() / 2.0);
  painter->fillPath(path6, QColor(161, 24, 15));
  painter->setPen(Qt::white);
  painter->drawText(tempR, 0, Zhu_Unicode);
  painter->restore();

  painter->restore();

  /******
   * 庚 酉鸡 辛
   * */
  painter->save();
  painter->rotate(OneEightCircleDegree * 4.0);

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  font.setPixelSize(fontPixSize1 * 3.0 / 4.0);
  painter->setFont(font);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Ren_Unicode);
  painter->drawText(
      QRectF(-base.x() + (base.x() * 2.0 / 3.0 - rect.width()) / 2.0,
             -base.y() + spacing1 / 4.0, rect.width(), rect.height()),
      0, Ren_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  font.setPixelSize(fontPixSize1);
  painter->setFont(font);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Zi_Unicode);
  painter->drawText(QRectF(-rect.width() / 2.0, -base.y() - spacing1 / 2.0,
                           rect.width(), rect.height()),
                    0, Zi_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  spacing2 = base.y() - fm.boundingRect(Zi_Unicode).height() - inner1.y();
  fontPixSize2 = inner1.y() - inner2.y() - spacing2;
  font.setPixelSize(fontPixSize2);
  painter->setFont(font);
  painter->setPen(QColor());
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Shu_Unicode);
  tempR = QRectF(-rect.width() / 2.0, -inner1.y() - rect.height() / 4.0,
                 rect.width(), rect.height());
  QPainterPath path7;
  path7.addRoundedRect(tempR.marginsAdded(QMarginsF(tempR.width() / 3.0, 0,
                                                    tempR.width() / 3.0, 0)),
                       tempR.width() / 2.0, tempR.height() / 2.0);
  painter->fillPath(path7, QColor(161, 24, 15));
  painter->setPen(Qt::white);
  painter->drawText(tempR, 0, Shu_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  font.setPixelSize(fontPixSize1 * 3.0 / 4.0);
  painter->setFont(font);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Gui_Unicode);
  painter->drawText(
      QRectF(base.x() / 3.0 + (base.x() * 2.0 / 3.0 - rect.width()) / 2.0,
             -base.y() + spacing1 / 4.0, rect.width(), rect.height()),
      0, Gui_Unicode);
  painter->restore();

  painter->restore();

  /******
   * 丑牛 艮 寅虎
   * */
  painter->save();
  painter->rotate(OneEightCircleDegree * 5.0);

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  font.setPixelSize(fontPixSize1);
  painter->setFont(font);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Chou_Unicode);
  painter->drawText(
      QRectF(-base.x() + (base.x() * 2.0 / 3.0 - rect.width()) / 2.0,
             -base.y() - spacing1 / 2.0, rect.width(), rect.height()),
      0, Chou_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  spacing2 = base.y() - fm.boundingRect(Chou_Unicode).height() - inner1.y();
  fontPixSize2 = inner1.y() - inner2.y() - spacing2;
  font.setPixelSize(fontPixSize2);
  painter->setFont(font);
  painter->setPen(QColor());
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Niu_Unicode);
  tempR =
      QRectF(-inner1.x() + (inner1.x() * 2.0 / 3.0 - rect.width()) / 2.0,
             -inner1.y() - rect.height() / 4.0, rect.width(), rect.height());
  QPainterPath path8;
  path8.addRoundedRect(tempR.marginsAdded(QMarginsF(tempR.width() / 3.0, 0,
                                                    tempR.width() / 3.0, 0)),
                       tempR.width() / 2.0, tempR.height() / 2.0);
  painter->fillPath(path8, QColor(161, 24, 15));
  painter->setPen(Qt::white);
  painter->drawText(tempR, 0, Niu_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  font.setPixelSize(fontPixSize1 * 3.0 / 4.0);
  painter->setFont(font);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Gen_Unicode);
  painter->drawText(QRectF(-rect.width() / 2.0, -base.y() - spacing1 / 2.0,
                           rect.width(), rect.height()),
                    0, Gen_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  font.setPixelSize(fontPixSize1);
  painter->setFont(font);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Yin_Unicode);
  painter->drawText(
      QRectF(base.x() / 3.0 + (base.x() * 2.0 / 3.0 - rect.width()) / 2.0,
             -base.y() - spacing1 / 2.0, rect.width(), rect.height()),
      0, Yin_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  spacing2 = base.y() - fm.boundingRect(Yin_Unicode).height() - inner1.y();
  fontPixSize2 = inner1.y() - inner2.y() - spacing2;
  font.setPixelSize(fontPixSize2);
  painter->setFont(font);
  painter->setPen(QColor());
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Hu_Unicode);
  tempR =
      QRectF(inner1.x() / 3.0 + (inner1.x() * 2.0 / 3.0 - rect.width()) / 2.0,
             -inner1.y() - rect.height() / 4.0, rect.width(), rect.height());
  QPainterPath path9;
  path9.addRoundedRect(tempR.marginsAdded(QMarginsF(tempR.width() / 3.0, 0,
                                                    tempR.width() / 3.0, 0)),
                       tempR.width() / 2.0, tempR.height() / 2.0);
  painter->fillPath(path9, QColor(161, 24, 15));
  painter->setPen(Qt::white);
  painter->drawText(tempR, 0, Hu_Unicode);
  painter->restore();

  painter->restore();

  /******
   * 甲 卯兔 乙
   * */
  painter->save();
  painter->rotate(OneEightCircleDegree * 6.0);

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  font.setPixelSize(fontPixSize1 * 3.0 / 4.0);
  painter->setFont(font);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Jia_Unicode);
  painter->drawText(
      QRectF(-base.x() + (base.x() * 2.0 / 3.0 - rect.width()) / 2.0,
             -base.y() + spacing1 / 4.0, rect.width(), rect.height()),
      0, Jia_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  font.setPixelSize(fontPixSize1);
  painter->setFont(font);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Mao_Unicode);
  painter->drawText(QRectF(-rect.width() / 2.0, -base.y() - spacing1 / 2.0,
                           rect.width(), rect.height()),
                    0, Mao_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  spacing2 = base.y() - fm.boundingRect(Mao_Unicode).height() - inner1.y();
  fontPixSize2 = inner1.y() - inner2.y() - spacing2;
  font.setPixelSize(fontPixSize2);
  painter->setFont(font);
  painter->setPen(QColor());
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Tu_Unicode);
  tempR = QRectF(-rect.width() / 2.0, -inner1.y() - rect.height() / 4.0,
                 rect.width(), rect.height());
  QPainterPath path10;
  path10.addRoundedRect(tempR.marginsAdded(QMarginsF(tempR.width() / 3.0, 0,
                                                     tempR.width() / 3.0, 0)),
                        tempR.width() / 2.0, tempR.height() / 2.0);
  painter->fillPath(path10, QColor(161, 24, 15));
  painter->setPen(Qt::white);
  painter->drawText(tempR, 0, Tu_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  font.setPixelSize(fontPixSize1 * 3.0 / 4.0);
  painter->setFont(font);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Yi_Unicode);
  painter->drawText(
      QRectF(base.x() / 3.0 + (base.x() * 2.0 / 3.0 - rect.width()) / 2.0,
             -base.y() + spacing1 / 4.0, rect.width(), rect.height()),
      0, Yi_Unicode);
  painter->restore();

  painter->restore();

  /******
   * 丑牛 艮 寅虎
   * */
  painter->save();
  painter->rotate(OneEightCircleDegree * 7.0);

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  font.setPixelSize(fontPixSize1);
  painter->setFont(font);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Chen_Unicode);
  painter->drawText(
      QRectF(-base.x() + (base.x() * 2.0 / 3.0 - rect.width()) / 2.0,
             -base.y() - spacing1 / 2.0, rect.width(), rect.height()),
      0, Chen_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  spacing2 = base.y() - fm.boundingRect(Chen_Unicode).height() - inner1.y();
  fontPixSize2 = inner1.y() - inner2.y() - spacing2;
  font.setPixelSize(fontPixSize2);
  painter->setFont(font);
  painter->setPen(QColor());
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Long_Unicode);
  tempR =
      QRectF(-inner1.x() + (inner1.x() * 2.0 / 3.0 - rect.width()) / 2.0,
             -inner1.y() - rect.height() / 4.0, rect.width(), rect.height());
  QPainterPath path11;
  path11.addRoundedRect(tempR.marginsAdded(QMarginsF(tempR.width() / 3.0, 0,
                                                     tempR.width() / 3.0, 0)),
                        tempR.width() / 2.0, tempR.height() / 2.0);
  painter->fillPath(path11, QColor(161, 24, 15));
  painter->setPen(Qt::white);
  painter->drawText(tempR, 0, Long_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  font.setPixelSize(fontPixSize1 * 3.0 / 4.0);
  painter->setFont(font);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Xun_Unicode);
  painter->drawText(QRectF(-rect.width() / 2.0, -base.y() - spacing1 / 2.0,
                           rect.width(), rect.height()),
                    0, Xun_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  font.setPixelSize(fontPixSize1);
  painter->setFont(font);
  fm = painter->fontMetrics();
  rect = fm.boundingRect(Si_Unicode);
  painter->drawText(
      QRectF(base.x() / 3.0 + (base.x() * 2.0 / 3.0 - rect.width()) / 2.0,
             -base.y() - spacing1 / 2.0, rect.width(), rect.height()),
      0, Si_Unicode);
  painter->restore();

  painter->save();
  font = painter->font();
  font.setFamily("KaiTi");
  font.setBold(true);
  spacing2 = base.y() - fm.boundingRect(Si_Unicode).height() - inner1.y();
  fontPixSize2 = inner1.y() - inner2.y() - spacing2;
  font.setPixelSize(fontPixSize2);
  painter->setFont(font);
  painter->setPen(QColor());
  fm = painter->fontMetrics();
  rect = fm.boundingRect(She_Unicode);
  tempR =
      QRectF(inner1.x() / 3.0 + (inner1.x() * 2.0 / 3.0 - rect.width()) / 2.0,
             -inner1.y() - rect.height() / 4.0, rect.width(), rect.height());
  QPainterPath path12;
  path12.addRoundedRect(tempR.marginsAdded(QMarginsF(tempR.width() / 3.0, 0,
                                                     tempR.width() / 3.0, 0)),
                        tempR.width() / 2.0, tempR.height() / 2.0);
  painter->fillPath(path12, QColor(161, 24, 15));
  painter->setPen(Qt::white);
  painter->drawText(tempR, 0, She_Unicode);
  painter->restore();

  painter->restore();
}

void Bagua::onTimer1Trick() {
  degree1_ += 5.0;
  update();
}
