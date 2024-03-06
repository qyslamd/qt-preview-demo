#pragma once
#include <QWidget>

class FourFishRotateProgress : public QWidget {
  Q_OBJECT
 public:
  explicit FourFishRotateProgress(QWidget *parent = nullptr);

 protected:
  void paintEvent(QPaintEvent *event) override;
  void drawProgress(QPainter *p);

 private:
  qreal size_base_ = 0.1;
  qreal size_div_ = 2.0;
  int tick_ = 0;

  // y=Asin(ωx+φ)+k
  // A:振幅
  // (ωx+φ):相位，反映y处的状态
  // k:反映在坐标系上图像的上移或下移
  const qreal A = 25.0;
  const qreal k = 10.0;

  QColor color1 = QColor(0x269DD9);
  QColor color2 = QColor(0xD9269D);
  QColor color3 = QColor(0xD9D926);
  QColor color4 = QColor(0x808080);
};
