#include <QCoreApplication>
#include <QTimer>

#include "process_manager.h"

int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);
  ProcessManager mgr;
  mgr.startProcess("conhost.exe", "", {});
  mgr.startProcess("mstsc.exe", "", {});

  QTimer::singleShot(5000, &app, QCoreApplication::quit);
  return app.exec();
}
