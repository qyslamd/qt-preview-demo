#pragma once

#include <QTabWidget>

class ClickWaterRipple;
class TestListView;
class Widget : public QTabWidget {
  Q_OBJECT
 public:
  Widget(QWidget *parent = nullptr);

 private:
  ClickWaterRipple *click_ripple_widget;
  TestListView *test_listview;
};
