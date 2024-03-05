#ifndef BARGUAROUND_H
#define BARGUAROUND_H

#include <QWidget>
#include <QOpenGLWidget>

class RotateData;
class QTimer;
class BaguaRound : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit BaguaRound(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
    void showEvent(QShowEvent *event) override;
private:
    void getBaguaPixmap(QPixmap &pixmap);

    void drawBackground(QPainter *painter, qreal radius);
    void drawYinYangFish(QPainter *painter, qreal radius);
    void drawGua(QPainter *painter, qreal radius);
    void drawGuaCharacter(QPainter *painter, qreal radius_outer, qreal radius_inner);
    void drawCircle(QPainter *painter, qreal radius, const QColor &color, qreal penWidth);
    void draw24SolarTerms(QPainter *painter, qreal radius_outer, qreal radius_inner);
    void drawScale(QPainter *painter, qreal radius_outer, qreal radius_inner);
    void drawTianGanDiZhi(QPainter *painter, qreal radius_outer, qreal radius_inner);
    void drawDiZhiWith5Elements(QPainter *painter, qreal radius_outer, qreal radius_inner);
    void drawTianGanWith5Elements(QPainter *painter, qreal radius_outer, qreal radius_inner);
    void drawOneEighthSeperator(QPainter *painter, qreal radius1, qreal radius2);
    void drawOneTwentyFourthSeperator(QPainter *painter, qreal radius1, qreal radius2);
    void drawQuarterSeperator(QPainter *painter, qreal radius1, qreal radius2);

private:
    QBrush background_brush_;
    RotateData *r_d_;
};

#endif // BARGUAROUND_H
