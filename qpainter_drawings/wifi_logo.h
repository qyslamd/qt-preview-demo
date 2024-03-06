#pragma once

#include <QWidget>

class TestWifiLogo : public QWidget {
  Q_OBJECT
 public:
  TestWifiLogo(QWidget *parent = nullptr);

 protected:
  void paintEvent(QPaintEvent *event) override;

 private:
  QPixmap pixmap;
  double ratio{0};
};
