#pragma once

#include <QWidget>

class QStateMachine;
class QState;
class QAbstractTransition;
class QTimer;
class AnimationPoint : public QObject {
  Q_OBJECT
  Q_PROPERTY(QPointF pos READ pos WRITE setPos NOTIFY posChanged)
 public:
  /*
   *        南
   *        ^
   *        |
   *        o
   *      o    o
   *    o   o   o
   *      o   o
   *        o
   *
   * */
  enum Gua {
    Li,    // 离 南
    Xun,   // 巽 东南
    Zhen,  // 震 东
    Gen,   // 艮 东北
    Kan,   // 坎 北
    Qian,  // 乾 西北
    Dui,   // 兑 西
    Kun,   // 坤 南西
  };
  AnimationPoint(Gua gua, const QPointF &start_pos, qreal square_width,
                 QObject *obj);
  static bool clockwise();

  QPointF pos() const;
  void setPos(const QPointF &pos);
  void setMidPoint(const QPoint &pos);
  void setSquareWidth(const int width);
 signals:
  void posChanged(const QPointF &pos);

 private:
  void setupStateMachine();
  void clearStateTransitions(QState *state);
  void clearTransitionAnime(QAbstractTransition *trans);

 private:
  static bool Clockwise;
  QPointF pos_;
  QTimer *timer_ = nullptr;
  Gua gua_ = Gua::Li;   // 用于确定起始位置
  QPointF mid_point_;   // 中心点的坐标，用于计算每个state的位置
  qreal square_width_;  // 正方形的边长

 private:
  QStateMachine *statem_;
  QState *state_li_;
  decltype(state_li_) state_xun_;
  decltype(state_li_) state_zhen_;
  decltype(state_li_) state_gen_;
  decltype(state_li_) state_kan_;
  decltype(state_li_) state_qian_;
  decltype(state_li_) state_dui_;
  decltype(state_li_) state_kun_;
};

class NineBallProgress : public QWidget {
  Q_OBJECT
 public:
  explicit NineBallProgress(QWidget *parent = nullptr);

 protected:
  void paintEvent(QPaintEvent *event) override;
 signals:

 private:
  AnimationPoint *d1_ = nullptr;
  AnimationPoint *d2_ = nullptr;
  AnimationPoint *d3_ = nullptr;
  AnimationPoint *d4_ = nullptr;
  AnimationPoint *d5_ = nullptr;
  AnimationPoint *d6_ = nullptr;
  AnimationPoint *d7_ = nullptr;
  AnimationPoint *d8_ = nullptr;

 private:
  void drawer1(QPainter *p);
};
