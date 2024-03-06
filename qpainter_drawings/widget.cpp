#include "widget.h"

#include <tabbarstyle.h>

#include <QTabBar>

#include "bagua.h"
#include "bagua_round.h"
#include "bagua_round_thread.h"
#include "cplusplus_logo.h"
#include "oval_seal.h"
#include "progress/ball_stair_progress.h"
#include "progress/four_fish_progress.h"
#include "progress/hourglass.h"
#include "progress/nine_ball_progress.h"
#include "progress/water_progress.h"
#include "progress/win10_loading_progress.h"
#include "speedometer.h"
#include "wifi_logo.h"

Widget::Widget(QWidget* parent)
    : QTabWidget(parent),
      bagua_(new Bagua(this)),
      round_bagua_(new BaguaRound(this)),
      round_bagua_thread(new BaguaRoundThread(this)),
      oval_seal(new OvalSealWidget(this)),
      hourglass(new HourglassWidget(this)),
      speedometer(new SpeedometerWidget(this)),
      win10_loading(new Win10LoadingProgressWidget(this)),
      water_progress(new WaterProgressDemo(this)),
      nine_ball_progress(new NineBallProgress),
      four_fish_progress(new FourFishRotateProgress(this)),
      ball_stair_progress(new ball_stair_progress::BallStairWidget(this)),
      cplusplus_logo(new CplusplusWidget(this)),
      wifi_logo(new TestWifiLogo(this)) {
  resize(1024, 768);

  this->tabBar()->setStyle(new TabBarStyle);

  addTab(win10_loading, "Win10 loading progress");
  addTab(bagua_, "Polygon bagua");
  addTab(round_bagua_, "Round bagua");
  addTab(round_bagua_thread, "Round bagua thread");
  addTab(oval_seal, "Oval seal");
  addTab(hourglass, "Hourglass");
  addTab(speedometer, "Speedometer");
  addTab(water_progress, "Water progress");
  addTab(nine_ball_progress, "Nine-ball progress");
  addTab(four_fish_progress, "Four-fish progress");
  addTab(ball_stair_progress, "Ball-stair progress");
  addTab(cplusplus_logo, "Cplusplus logo");
  addTab(wifi_logo, "Wifi logo");
}
