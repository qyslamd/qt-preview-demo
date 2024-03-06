#pragma once

#include <QWidget>

class QVariantAnimation;
class Win10LoadingProgressWidget : public QWidget {
  Q_OBJECT

 public:
  Win10LoadingProgressWidget(QWidget* parent = nullptr);
  ~Win10LoadingProgressWidget() override;

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  QVariantAnimation* anime_ = nullptr;
  qreal t_ = 0.0;

 private:
  qreal currentValue();
};
