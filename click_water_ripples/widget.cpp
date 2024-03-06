#include "widget.h"

#include "click_ripple_listview.h"
#include "click_water_ripple_widget.h"

Widget::Widget(QWidget *parent)
    : QTabWidget(parent),
      click_ripple_widget(new ClickWaterRipple(this)),
      test_listview(new TestListView(this)) {
  addTab(click_ripple_widget, "Click ripple widget");
  addTab(test_listview, "Click ripple listview");
}
