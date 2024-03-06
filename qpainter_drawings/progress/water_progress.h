#pragma once

#include <QScopedPointer>
#include <QWidget>

class WaterProgressPrivate;
class WaterProgress : public QWidget {
  Q_OBJECT

  Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)
 public:
  explicit WaterProgress(QWidget *parent = nullptr);
  ~WaterProgress() override;

  int value() const;

 signals:
  void valueChanged();

 public slots:
  void start();
  void stop();
  void setValue(int value);
  void setTextVisible(bool visible);

 protected:
  void paintEvent(QPaintEvent *) override;

 private slots:
  void resizeEvent(QResizeEvent *event) override;

 private:
  WaterProgressPrivate *d;
};

class QSlider;
class WaterProgressDemo : public QWidget {
  Q_OBJECT
 public:
  WaterProgressDemo(QWidget *parent = nullptr);

 private:
  WaterProgress *progress = nullptr;
  QSlider *horizontalSlider = nullptr;
};
