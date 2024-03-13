#include "app_settings.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QtDebug>

#ifdef USE_CPP17
#include <filesystem>
#endif

namespace {
auto Group_Window = "Window";
auto Key_CentralSplitter = "CentralSplitter";
auto Key_WindowGeometry = "MainwindowGeometry";
auto Key_tabWidgetSplitter = "TabwidgetSplitter";
}  // namespace

AppSettings::AppSettings(QObject *parent) : QObject{parent} {
  QString filePath;
#ifndef USE_CPP17
  filePath = QApplication::applicationDirPath();
  if (filePath.contains("/")) {
    filePath.append("/config/").append("config.ini");
  } else {
    filePath.append("\\config\\").append("config.ini");
  }
#else
  namespace fs = std::filesystem;
  filePath = QString::fromStdString(
      fs::path(fs::current_path() / "config/config.ini").generic_string());
#endif
  settings_ = new QSettings(filePath, QSettings::IniFormat, this);

  // TODO: you can create default value here;
}

AppSettings &AppSettings::instance() {
  static AppSettings gInstant;
  return gInstant;
}

QVariant AppSettings::value(const QString &key) {
  return settings_->value(key);
}

QVariant AppSettings::value(const QString &group, const QString &key) {
  settings_->beginGroup(group);
  auto ret = settings_->value(key);
  settings_->endGroup();
  return ret;
}

void AppSettings::setValue(const QString &key, const QVariant &value) {
  settings_->setValue(key, value);
}

void AppSettings::setValue(const QString &group, const QString &key,
                           const QVariant &value) {
  settings_->beginGroup(group);
  settings_->setValue(key, value);
  settings_->endGroup();
}

QByteArray AppSettings::centralSplitterState() {
  return instance().value(Group_Window, Key_CentralSplitter).toByteArray();
}

void AppSettings::saveCentralSplitterState(const QByteArray &data) {
  instance().setValue(Group_Window, Key_CentralSplitter, data);
}

QByteArray AppSettings::tabwidgetSplitterState() {
  return instance().value(Group_Window, Key_tabWidgetSplitter).toByteArray();
}

void AppSettings::saveTabwidgetSplitterState(const QByteArray &data) {
  instance().setValue(Group_Window, Key_tabWidgetSplitter, data);
}

QByteArray AppSettings::windowGeometry() {
  return instance().value(Group_Window, Key_WindowGeometry).toByteArray();
}

void AppSettings::setWindowGeometry(const QByteArray &data) {
  instance().setValue(Group_Window, Key_WindowGeometry, data);
}
