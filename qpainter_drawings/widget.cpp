#include "widget.h"

#include <tabbarstyle.h>

#include <QTabBar>

#include "bagua.h"
#include "bagua_round.h"
#include "hourglass.h"
#include "oval_seal.h"
#include "speedometer.h"
#include "win10_loading_progress.h"

Widget::Widget(QWidget* parent)
    : QTabWidget(parent),
      bagua_(new Bagua(this)),
      round_bagua_(new BaguaRound(this)),
      oval_seal(new OvalSealWidget(this)),
      hourglass(new HourglassWidget(this)),
      speedometer(new SpeedometerWidget(this)),
      win10_loading(new Win10LoadingProgressWidget(this)) {
  resize(1024, 768);

  this->tabBar()->setStyle(new TabBarStyle);

  addTab(bagua_, "Polygon Bagua");
  addTab(round_bagua_, "Round Bagua");
  addTab(oval_seal, "Oval Seal");
  addTab(hourglass, "Hourglass");
  addTab(speedometer, "Speedometer");
  addTab(win10_loading, "Win10 Loading Progress");
}
