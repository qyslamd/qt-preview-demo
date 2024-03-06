#pragma once

#include <QWidget>

class QTimer;
class HourglassWidget : public QWidget {
  Q_OBJECT
 public:
  explicit HourglassWidget(QWidget* parent = nullptr);

 protected:
  void paintEvent(QPaintEvent*) override;

 private:
  enum { State_UpTriangle = 0, State_DownTriangle, DRAW_CIR_ROTATE };

  int now_progress_;
  int frame_width_ = 6;
  QColor frame_color_ = QColor(63, 107, 157, 255);
  QColor sand_color_ = QColor(222, 164, 96);
  int rotate_speed_ = 10;
  int sand_drop_speed_ = 10;

  QTimer* timer_ = nullptr;
  bool is_first_rotate_;
  qreal rotate_angle_ = 0;
  int now_status_ = 2;
  bool is_drawing_triangle_ = false;
  qreal triangleH;

 private:
  void drawSandClock(QPainter* painter);
 private slots:
  void UpdateAngle();
};
