#include <QCoreApplication>
#include <QTimer>

#include "process_manager.h"

int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);
  ProcessManager mgr;
  mgr.startProcess(R"(C:\Program Files (x86)\Notepad++\notepad++.exe)", "", {});
  mgr.startProcess("regedit.exe", "", {});

  QTimer::singleShot(5000, &app, QCoreApplication::quit);
  return app.exec();
}
