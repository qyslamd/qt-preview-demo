#include "process_manager.h"

#include <QMetaEnum>
#include <algorithm>
#include <iostream>

ProcessManager::ProcessManager(QObject *parent) : QObject{parent} {}

ProcessManager::~ProcessManager() {
  std::cout << __FUNCTION__ << std::endl;
  for (auto const &item : processList) {
    item->disconnect();  // important!
    item->kill();
    item->waitForFinished();
  }
  processList.clear();
}

void ProcessManager::startProcess(const QString &program,
                                  const QString &workDir,
                                  const QStringList &args) {
  std::shared_ptr<QProcess> process(new MyProcess);
  processList.insert(process);

  connect(process.get(), &QProcess::started, this,
          &ProcessManager::onProcessStarted);
  connect(process.get(),
          QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
          &ProcessManager::onProcessFinished);
  connect(process.get(), &QProcess::errorOccurred, this,
          &ProcessManager::onProcessErrorOccured);

  process->setProgram(program);
  process->setWorkingDirectory(workDir);
  process->setArguments(args);

  process->start();
  process->waitForStarted();
}

void ProcessManager::onProcessStarted() {
  std::cout << __FUNCTION__ << std::endl;
}

void ProcessManager::onProcessErrorOccured(QProcess::ProcessError error) {
  auto type = QMetaEnum::fromType<QProcess::ProcessError>();
  std::cout << __FUNCTION__ << " ProcessError: " << type.valueToKey(error)
            << std::endl;
  removeProcess(qobject_cast<QProcess *>(QObject::sender()));
}

void ProcessManager::onProcessFinished(int exitCode,
                                       QProcess::ExitStatus status) {
  auto type = QMetaEnum::fromType<QProcess::ExitStatus>();
  std::cout << __FUNCTION__ << " exitCode: " << exitCode
            << " ExitStatus: " << type.valueToKey(status) << std::endl;
  removeProcess(qobject_cast<QProcess *>(QObject::sender()));
}

void ProcessManager::removeProcess(QProcess *process) {
  for (auto it = processList.begin(); it != processList.end();) {
    if ((*it).get() == process) {
      processList.erase(it++);
    } else {
      it++;
    }
  }
}
