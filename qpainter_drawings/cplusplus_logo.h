#pragma once

#include <QWidget>

class CplusplusWidget : public QWidget {
  Q_OBJECT
 public:
  explicit CplusplusWidget(QWidget *parent = nullptr);

 protected:
  void paintEvent(QPaintEvent *event) override;

 private:
  qreal t_;
  int count_ = 6;
};
