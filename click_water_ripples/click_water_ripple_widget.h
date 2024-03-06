#pragma once

#include <QWidget>

class WaterRipplePrivate;
class ClickWaterRipple : public QWidget {
  Q_OBJECT
 public:
  ClickWaterRipple(QWidget *parent = nullptr);
  ~ClickWaterRipple();

 protected:
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;

 private:
  WaterRipplePrivate *d_ = nullptr;

  Q_DISABLE_COPY(ClickWaterRipple)
};
