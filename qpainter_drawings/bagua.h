#ifndef BAGUA_H
#define BAGUA_H

#include <QWidget>

class Bagua : public QWidget {
  Q_OBJECT
 public:
  explicit Bagua(QWidget *parent = nullptr);

 protected:
  void paintEvent(QPaintEvent *event) override;
  void drawFishCircle(QPainter *painter);
  void drawFramesAndCharacters(QPainter *painter);

 private:
  void drawGua(QPainter *painter, qreal a, qreal b);
  void drawOctagon(QPainter *painter, qreal penWidth, const QColor &color,
                   qreal a, qreal b);
  void drawSpacer(QPainter *painter, qreal penWidth, const QColor &color,
                  const QLineF &line);
  void drawGuaCharacter(QPainter *painter, const QPointF &outer,
                        const QPointF &base, const QPointF &inner);
  void drawDirectionAndElement(QPainter *painter, const QPointF &outer,
                               const QPointF &base, const QPointF &inner);
  void drawTianGanAndDiZhi(QPainter *painter, const QPointF &outer,
                           const QPointF &base, const QPointF &inner1,
                           const QPointF &inner2);

 private:
  QTimer *timer_1;
  int degree1_ = 0;

  // Color of gua
  QColor color_gua_li_{0xDC2718};
  QColor color_gua_kun_{0xF8F40E};
  QColor color_gua_dui_{0xCBCBC9};
  QColor color_gua_qian_{0x25185E};
  QColor color_gua_kan_{0x8B4169};
  QColor color_gua_gen_{0x138BCC};
  QColor color_gua_zhen_{0x0D8942};
  QColor color_gua_xun_{0xE67A19};

  // 五层八边形的颜色，从外层到内层
  QColor color_frame_1_{0xFE0000};
  QColor color_frame_2_{0xFF6300};
  QColor color_frame_3_{0x4B0081};
  QColor color_frame_4_{0x008001};
  QColor color_frame_5_{0xBC31FE};

  // 五层八边形的颜色，从外层到内层
  QColor color_spacer_1_{0x00FFFF};
  QColor color_spacer_2_{0x00FA9A};
  QColor color_spacer_3_{0x4B0081};
  QColor color_spacer_4_{0xDAA520};
  QColor color_spacer_5_{0x556B2F};
  QColor color_spacer_6_{0x40E0D0};
  QColor color_spacer_7_{0x4169E1};
  QColor color_spacer_8_{0x800080};
 private slots:
  void onTimer1Trick();
};

#endif  // BAGUA_H
