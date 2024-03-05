#pragma once
#include <QProxyStyle>

class TabBarStyle : public QProxyStyle {
 public:
  TabBarStyle();

  // QStyle interface
 public:
  QSize sizeFromContents(ContentsType type, const QStyleOption *option,
                         const QSize &size,
                         const QWidget *widget) const override;
  void drawControl(ControlElement element, const QStyleOption *opt, QPainter *p,
                   const QWidget *w) const override;

 private:
  void drawTabBarTabShape(const QStyleOption *option, QPainter *painter,
                          const QWidget *w) const;
  QPainterPath getSelectedShape(const QStyleOption *option) const;
  QPainterPath getHoveredShape(const QStyleOption *option) const;
  QLineF getDividingLine(const QStyleOption *option) const;

 private:
  const qreal topMargin = 0.0;
  const qreal HRatio = 1.0 / 5.0;
};
