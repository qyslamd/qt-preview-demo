#pragma once
#include <QWidget>

class QStateMachine;
class QState;
class QAbstractTransition;
class QAbstractAnimation;
class QTimer;

namespace ball_stair_progress {
class AnimationPoint : public QObject {
  Q_OBJECT
  Q_PROPERTY(QPointF pos READ pos WRITE setPos NOTIFY posChanged)
 public:
  /*
   *    o                 o
   *    - o             o -
   *    - - o         o - -
   *    - - - o     o - - -
   *    - - - - o o - - - -
   *    - - - - - - - - - -
   * */
  explicit AnimationPoint(const QRectF &rect, QObject *parent = nullptr);
  ~AnimationPoint();

  QPointF pos() const;
  void setPos(const QPointF &pos);

  qreal minHLen() const;
  qreal minWLen() const;
  qreal spacing() const;
 signals:
  void posChanged(const QPointF &pos);
  void revertChanged(bool revert);

 private:
  void setupStateMachine();
  void clearStateTransitions(QState *state);
  void clearTransitionAnime(QAbstractTransition *trans);

  QAbstractAnimation *createVariantAnimation(const QVariant &start,
                                             const QVariant &end);

 private:
  QPointF pos_;
  QTimer *timer_ = nullptr;
  QRectF rect_;  // 绘制范围矩形
  qreal min_h_len_;
  qreal min_w_len_;
  qreal spacing_;

 private:
  QStateMachine *statem_ = nullptr;
  static const int Size = 10;
  QState *array_state_[Size];
  QPointF array_pos_[Size];
};

class BallStairWidget : public QWidget {
  Q_OBJECT
 public:
  explicit BallStairWidget(QWidget *parent = nullptr);

 protected:
  void paintEvent(QPaintEvent *event) override;
  void drawProgress(QPainter *p);

 private:
  AnimationPoint *point_ = nullptr;
  bool need_revert_ = true;
};

}  // namespace ball_stair_progress
