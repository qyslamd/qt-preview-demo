#pragma once

#include <QWidget>

class SpeedometerWidget : public QWidget {
  Q_OBJECT
  Q_ENUMS(PieStyle)
  Q_ENUMS(PointerStyle)

  Q_PROPERTY(double minValue READ getMinValue WRITE setMinValue)
  Q_PROPERTY(double maxValue READ getMaxValue WRITE setMaxValue)
  Q_PROPERTY(double value READ getValue WRITE setValue)
  Q_PROPERTY(int precision READ getPrecision WRITE setPrecision)

  Q_PROPERTY(int scaleMajor READ getScaleMajor WRITE setScaleMajor)
  Q_PROPERTY(int scaleMinor READ getScaleMinor WRITE setScaleMinor)
  Q_PROPERTY(int startAngle READ getStartAngle WRITE setStartAngle)
  Q_PROPERTY(int endAngle READ getEndAngle WRITE setEndAngle)

  Q_PROPERTY(bool animation READ getAnimation WRITE setAnimation)
  Q_PROPERTY(double animationStep READ getAnimationStep WRITE setAnimationStep)

  Q_PROPERTY(QColor outerCircleColor READ getOuterCircleColor WRITE
                 setOuterCircleColor)
  Q_PROPERTY(QColor innerCircleColor READ getInnerCircleColor WRITE
                 setInnerCircleColor)

  Q_PROPERTY(QColor pieColorStart READ getPieColorStart WRITE setPieColorStart)
  Q_PROPERTY(QColor pieColorMid READ getPieColorMid WRITE setPieColorMid)
  Q_PROPERTY(QColor pieColorEnd READ getPieColorEnd WRITE setPieColorEnd)

  Q_PROPERTY(QColor coverCircleColor READ getCoverCircleColor WRITE
                 setCoverCircleColor)
  Q_PROPERTY(QColor scaleColor READ getScaleColor WRITE setScaleColor)
  Q_PROPERTY(QColor pointerColor READ getPointerColor WRITE setPointerColor)
  Q_PROPERTY(QColor centerCircleColor READ getCenterCircleColor WRITE
                 setCenterCircleColor)
  Q_PROPERTY(QColor textColor READ getTextColor WRITE setTextColor)

  Q_PROPERTY(bool showOverlay READ getShowOverlay WRITE setShowOverlay)
  Q_PROPERTY(QColor overlayColor READ getOverlayColor WRITE setOverlayColor)

  Q_PROPERTY(PieStyle pieStyle READ getPieStyle WRITE setPieStyle)
  Q_PROPERTY(
      PointerStyle pointerStyle READ getPointerStyle WRITE setPointerStyle)

 public:
  enum PieStyle {
    PieStyle_Three = 0,   //三色圆环
    PieStyle_Current = 1  //当前圆环
  };

  enum PointerStyle {
    PointerStyle_Circle = 0,      //圆形指示器
    PointerStyle_Indicator = 1,   //指针指示器
    PointerStyle_IndicatorR = 2,  //圆角指针指示器
    PointerStyle_Triangle = 3     //三角形指示器
  };

  explicit SpeedometerWidget(QWidget* parent = nullptr);
  ~SpeedometerWidget();
 signals:
  void valueChanged(int value);

 protected:
  void paintEvent(QPaintEvent*);
  void drawOuterCircle(QPainter* painter);
  void drawInnerCircle(QPainter* painter);
  void drawColorPie(QPainter* painter);
  void drawCoverCircle(QPainter* painter);
  void drawScale(QPainter* painter);
  void drawScaleNum(QPainter* painter);
  void drawPointerCircle(QPainter* painter);
  void drawPointerIndicator(QPainter* painter);
  void drawPointerIndicatorR(QPainter* painter);
  void drawPointerTriangle(QPainter* painter);
  void drawRoundCircle(QPainter* painter);
  void drawCenterCircle(QPainter* painter);
  void drawValue(QPainter* painter);
  void drawOverlay(QPainter* painter);

 private slots:
  void updateValue();

 private:
  double minValue = 0;    //最小值
  double maxValue = 100;  //最大值
  double value = 60;      //目标值
  int precision = 0;      //精确度,小数点后几位

  int scaleMajor = 10;  //大刻度数量
  int scaleMinor = 10;  //小刻度数量
  int startAngle = 45;  //开始旋转角度
  int endAngle = 45;    //结束旋转角度

  bool animation = false;      //是否启用动画显示
  double animationStep = 500;  //动画显示时步长

  QColor outerCircleColor = QColor(0x505050);  //外圆背景颜色
  QColor innerCircleColor = QColor(0x313237);  //内圆背景颜色

  QColor pieColorStart = QColor(0x18BC9A);  //饼圆开始颜色
  QColor pieColorMid = QColor(0xD9D900);    //饼圆中间颜色
  QColor pieColorEnd = QColor(0xFE6A6A);    //饼圆结束颜色

  QColor coverCircleColor = innerCircleColor;  //覆盖圆背景颜色
  QColor scaleColor = Qt::white;               //刻度尺颜色
  QColor pointerColor = QColor(0xF76A6A);      //指针颜色
  QColor centerCircleColor = Qt::white;        //中心圆颜色
  QColor textColor = Qt::black;                //文字颜色

  bool showOverlay = true;                           //显示遮罩层
  QColor overlayColor = QColor(192, 192, 192, 100);  //遮罩层颜色

  PieStyle pieStyle = PieStyle_Three;                  //饼图样式
  PointerStyle pointerStyle = PointerStyle_Indicator;  //指针样式

  bool reverse = true;       //是否往回走
  double currentValue = 55;  //当前值
  QTimer* timer;             //定时器绘制动画

 public:
  double getMinValue() const;
  double getMaxValue() const;
  double getValue() const;
  int getPrecision() const;

  int getScaleMajor() const;
  int getScaleMinor() const;
  int getStartAngle() const;
  int getEndAngle() const;

  bool getAnimation() const;
  double getAnimationStep() const;

  QColor getOuterCircleColor() const;
  QColor getInnerCircleColor() const;

  QColor getPieColorStart() const;
  QColor getPieColorMid() const;
  QColor getPieColorEnd() const;

  QColor getCoverCircleColor() const;
  QColor getScaleColor() const;
  QColor getPointerColor() const;
  QColor getCenterCircleColor() const;
  QColor getTextColor() const;

  bool getShowOverlay() const;
  QColor getOverlayColor() const;

  PieStyle getPieStyle() const;
  PointerStyle getPointerStyle() const;

  QSize sizeHint() const;
  QSize minimumSizeHint() const;

 public Q_SLOTS:
  //设置范围值
  void setRange(double minValue, double maxValue);
  void setRange(int minValue, int maxValue);

  //设置最大最小值
  void setMinValue(double minValue);
  void setMaxValue(double maxValue);

  //设置目标值
  void setValue(double value);
  void setValue(int value);

  //设置精确度
  void setPrecision(int precision);

  //设置主刻度数量
  void setScaleMajor(int scaleMajor);
  //设置小刻度数量
  void setScaleMinor(int scaleMinor);
  //设置开始旋转角度
  void setStartAngle(int startAngle);
  //设置结束旋转角度
  void setEndAngle(int endAngle);

  //设置是否启用动画显示
  void setAnimation(bool animation);
  //设置动画显示的步长
  void setAnimationStep(double animationStep);

  //设置外圆背景颜色
  void setOuterCircleColor(const QColor& outerCircleColor);
  //设置内圆背景颜色
  void setInnerCircleColor(const QColor& innerCircleColor);

  //设置饼圆三种颜色
  void setPieColorStart(const QColor& pieColorStart);
  void setPieColorMid(const QColor& pieColorMid);
  void setPieColorEnd(const QColor& pieColorEnd);

  //设置覆盖圆背景颜色
  void setCoverCircleColor(const QColor& coverCircleColor);
  //设置刻度尺颜色
  void setScaleColor(const QColor& scaleColor);
  //设置指针颜色
  void setPointerColor(const QColor& pointerColor);
  //设置中心圆颜色
  void setCenterCircleColor(const QColor& centerCircleColor);
  //设置文本颜色
  void setTextColor(const QColor& textColor);

  //设置是否显示遮罩层
  void setShowOverlay(bool showOverlay);
  //设置遮罩层颜色
  void setOverlayColor(const QColor& overlayColor);

  //设置饼图样式
  void setPieStyle(const PieStyle& pieStyle);
  //设置指针样式
  void setPointerStyle(const PointerStyle& pointerStyle);
};
