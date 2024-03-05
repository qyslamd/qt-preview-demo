#ifndef WIDGET_H
#define WIDGET_H

#include <QTabWidget>

class Bagua;
class BaguaRound;
class Widget : public QTabWidget {
  Q_OBJECT
 public:
  Widget(QWidget *parent = nullptr);

 private:
  Bagua *bagua_ = nullptr;
  BaguaRound *round_bagua_ = nullptr;
};
#endif  // WIDGET_H
