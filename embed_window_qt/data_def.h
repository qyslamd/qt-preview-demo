#ifndef DATA_DEF_H
#define DATA_DEF_H

#include <QString>
#include <QVariant>
#include <QWidget>
#include <QtDebug>

struct ProcessConfiguration {
  int id;
  QString alias;
  QString process_name;
  QString process_path;
  QStringList process_arguments;
  QString window_title;
  QString wnd_class_name;
  int waiting_time;
  bool kill_and_start;
};
Q_DECLARE_METATYPE(ProcessConfiguration);

struct EmbededInfo {
  ProcessConfiguration cfg;
  QWindow *qt_widow;
  long gml_style_before_embeded;
  void *native_menu;
};

Q_DECLARE_METATYPE(EmbededInfo);

bool cfgEquals(const ProcessConfiguration &lhs,
               const ProcessConfiguration &rhs);

QStringList GetArguments(const QString &para);

QStringList GetArgumentsOrdered(const QString &para);

#endif  // DATA_DEF_H
