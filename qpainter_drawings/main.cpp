#include <QApplication>
#include <QFont>
#include <QTranslator>

#include "widget.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  a.setFont(QFont("Microsoft YaHei"));
  QTranslator ts;
  ts.load(":/resource/zh_CN.qm");
  a.installTranslator(&ts);

  Widget w;
  w.show();

  return a.exec();
}
