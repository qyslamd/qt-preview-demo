#include "widget.h"

#include <tabbarstyle.h>

#include <QTabBar>

#include "bagua.h"
#include "bagua_round.h"
#include "oval_seal.h"

Widget::Widget(QWidget *parent)
    : QTabWidget(parent),
      bagua_(new Bagua(this)),
      round_bagua_(new BaguaRound(this)),
      oval_seal(new OvalSealWidget(this)) {
  resize(1024, 768);

  this->tabBar()->setStyle(new TabBarStyle);

  addTab(bagua_, "Polygon Bagua");
  addTab(round_bagua_, "Round Bagua");
  addTab(oval_seal, "Oval Seal");
}
