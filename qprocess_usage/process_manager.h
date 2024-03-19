#pragma once

#include <QObject>
#include <QProcess>
#include <QVector>
#include <iostream>
#include <memory>
#include <set>

class MyProcess : public QProcess {
  Q_OBJECT
 public:
  MyProcess(QObject *parent = nullptr) : QProcess(parent) {}
  ~MyProcess() override { std::cout << __FUNCTION__ << std::endl; }
};

class ProcessManager : public QObject {
  Q_OBJECT
 public:
  explicit ProcessManager(QObject *parent = nullptr);
  ~ProcessManager() override;

  void startProcess(const QString &program, const QString &workDir,
                    const QStringList &args);
 signals:
 private slots:
  void onProcessStarted();
  void onProcessErrorOccured(QProcess::ProcessError error);
  void onProcessFinished(int exitCode, QProcess::ExitStatus status);

 private:
  void removeProcess(QProcess *process);

 private:
  std::set<std::shared_ptr<QProcess>> processList;
};
