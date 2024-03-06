#pragma once

#include <QTabWidget>

class Bagua;
class BaguaRound;
class BaguaRoundThread;
class OvalSealWidget;
class HourglassWidget;
class SpeedometerWidget;
class Win10LoadingProgressWidget;
class WaterProgressDemo;
class NineBallProgress;
class FourFishRotateProgress;
namespace ball_stair_progress {
class BallStairWidget;
}
class CplusplusWidget;
class TestWifiLogo;
class Widget : public QTabWidget {
  Q_OBJECT
 public:
  Widget(QWidget* parent = nullptr);

 private:
  Bagua* bagua_ = nullptr;
  BaguaRound* round_bagua_ = nullptr;
  BaguaRoundThread* round_bagua_thread = nullptr;
  OvalSealWidget* oval_seal = nullptr;
  HourglassWidget* hourglass = nullptr;
  SpeedometerWidget* speedometer = nullptr;
  Win10LoadingProgressWidget* win10_loading = nullptr;
  WaterProgressDemo* water_progress = nullptr;
  NineBallProgress* nine_ball_progress = nullptr;
  FourFishRotateProgress* four_fish_progress = nullptr;
  ball_stair_progress::BallStairWidget* ball_stair_progress = nullptr;
  CplusplusWidget* cplusplus_logo = nullptr;
  TestWifiLogo* wifi_logo = nullptr;
};
