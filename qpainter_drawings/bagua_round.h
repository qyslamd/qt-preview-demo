#pragma once
#include <QOpenGLWidget>
#include <QWidget>

class RotateData;
class QTimer;
class BaguaRound : public QWidget {
  Q_OBJECT
 public:
  explicit BaguaRound(QWidget *parent = nullptr);

 protected:
  void paintEvent(QPaintEvent *event) override;

 private:
  void getBaguaPixmap(QPixmap &pixmap);

  void drawBackground(QPainter *painter, qreal radius);
  void drawYinYangFish(QPainter *painter, qreal radius);
  void drawGua(QPainter *painter, qreal radius);
  void drawGuaCharacter(QPainter *painter, qreal radius_outer,
                        qreal radius_inner);
  void drawCircle(QPainter *painter, qreal radius, const QColor &color,
                  qreal penWidth);
  void draw24SolarTerms(QPainter *painter, qreal radius_outer,
                        qreal radius_inner);
  void drawScale(QPainter *painter, qreal radius_outer, qreal radius_inner);
  void drawTianGanDiZhi(QPainter *painter, qreal radius_outer,
                        qreal radius_inner);
  void drawDiZhiWith5Elements(QPainter *painter, qreal radius_outer,
                              qreal radius_inner);
  void drawTianGanWith5Elements(QPainter *painter, qreal radius_outer,
                                qreal radius_inner);
  void drawOneEighthSeperator(QPainter *painter, qreal radius1, qreal radius2);
  void drawOneTwentyFourthSeperator(QPainter *painter, qreal radius1,
                                    qreal radius2);
  void drawQuarterSeperator(QPainter *painter, qreal radius1, qreal radius2);

 private:
  RotateData *r_d_;
};
