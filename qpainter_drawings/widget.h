#pragma once

#include <QTabWidget>

class Bagua;
class BaguaRound;
class OvalSealWidget;
class Widget : public QTabWidget {
  Q_OBJECT
 public:
  Widget(QWidget *parent = nullptr);

 private:
  Bagua *bagua_ = nullptr;
  BaguaRound *round_bagua_ = nullptr;
  OvalSealWidget *oval_seal = nullptr;
};
