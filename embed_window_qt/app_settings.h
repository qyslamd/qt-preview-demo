#pragma once

#include <QSettings>

class AppSettings : public QObject {
  Q_OBJECT
 public:
  static QByteArray centralSplitterState();
  static void saveCentralSplitterState(const QByteArray &data);

  static QByteArray tabwidgetSplitterState();
  static void saveTabwidgetSplitterState(const QByteArray &data);

  static QByteArray windowGeometry();
  static void setWindowGeometry(const QByteArray &data);

 private:
  explicit AppSettings(QObject *parent = nullptr);
  static AppSettings &instance();
 signals:

 private:
  QVariant value(const QString &key);
  QVariant value(const QString &group, const QString &key);

  void setValue(const QString &key, const QVariant &value);
  void setValue(const QString &group, const QString &key,
                const QVariant &value);

 private:
  QSettings *settings_ = nullptr;
};
