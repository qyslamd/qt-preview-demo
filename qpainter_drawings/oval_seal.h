#pragma once

#include <QWidget>

class OvalSealWidget : public QWidget {
  Q_OBJECT
 public:
  OvalSealWidget(QWidget *parent = nullptr);

 protected:
  void paintEvent(QPaintEvent *event) override;

 private:
  void paintProc2(QPainter *p);
};
