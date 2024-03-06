#include <QApplication>
#include <QWidget>

#include "click_water_ripple_widget.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  ClickWaterRipple w;
  w.show();
  return app.exec();
}
