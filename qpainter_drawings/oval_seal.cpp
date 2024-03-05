#include "oval_seal.h"

#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>

OvalSealWidget::OvalSealWidget(QWidget *parent) : QWidget(parent) {}

void OvalSealWidget::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);

  QPainter p(this);
  paintProc2(&p);
}

// clang-format off
void OvalSealWidget::paintProc2(QPainter *p)
{
  auto rect = this->rect();
  p->fillRect(rect,Qt::white);

  auto minorAxisLen = std::min(rect.width(), rect.height());

  p->save();
  p->translate(rect.center());

  // 绘制圆心辅助观察
  //    p->save();
  //    p->setPen(QPen(Qt::red, 4));
  //    p->drawPoint(0, 0);
  //    p->restore();

  // 把原来的矩形移动到当前移动后的坐标系中，矩形的中点在坐标原点
  auto rectNew = rect.translated(-rect.center());

  // 绘制椭圆1
  p->save();
  p->setPen(QPen(QColor(0xF90808), minorAxisLen / 30.0));
  QPainterPath path;
  path.addEllipse(rectNew);
  p->drawPath(path);
  p->restore();

  // 绘制椭圆2
  //    p->save();
  //    p->setPen(QPen(QColor(0xF90808), minorAxisLen / 30.0));
  //    QPainterPath path2;
  //    QRect rect2;
  //    rect2.setWidth(rect.width() / 8 * 6);
  //    rect2.setHeight(rect.height() / 8 * 6);
  //    rect2.moveCenter(QPoint()); // 当前坐标原点在矩形的中心
  //    path2.addEllipse(rect2);
  //    p->drawPath(path2);
  //    p->restore();

  // 绘制中间的文字
  p->save();
  p->setPen(QPen(QColor(0xF90808), minorAxisLen / 30.0));
  auto fontTemp = p->font();
  fontTemp.setPixelSize(minorAxisLen / 4.2);
  p->setFont(fontTemp);
  const QString text("NBA");
  auto textBoundingRect = p->fontMetrics().boundingRect(text);
  auto destRect = textBoundingRect;
  destRect.moveCenter(QPoint());
  p->translate(QPoint(0, -destRect.height() / 4));
  p->drawText(destRect,0, text, &textBoundingRect);

  const QString text2("WWF");
  auto fontTemp2 = p->font();
  fontTemp2.setPixelSize(minorAxisLen / 6);
  p->setFont(fontTemp2);
  auto textBoundingRect2 = p->fontMetrics().boundingRect(text2);
  auto destRect2 = textBoundingRect2;
  destRect2.moveCenter(QPoint());
  p->translate(QPoint(0, destRect.height() / 4 + destRect2.height()));
  p->drawText(destRect2,0, text2, &textBoundingRect2);

  p->restore();

  // 辅助观察百分比的位置
  auto test = [=](){
      auto p0 = QPointF(0, 0) ;

      p->save();
      p->setPen(QPen(QColor(Qt::black), 1, Qt::DashLine));
      p->drawLine(p0, path.pointAtPercent(0));
      p->restore();

      p->save();
      p->setPen(QPen(QColor(Qt::red), 1, Qt::DashLine));
      p->drawLine(p0, path.pointAtPercent(0.5));
      p->restore();
  };

#if 1
  /******************绘制上边字**********************/
  p->save();
  //    test();

  const QString characters("深圳腾讯科技有限公司");

  // 首先确定字体大小，字体大小决定了绘制的弧长
  QFont font = p->font();
  font.setPixelSize(minorAxisLen / 6.0);
  font.setBold(true);
  p->setFont(font);
  auto ww = p->fontMetrics().width(characters.at(0));
  auto hh = p->fontMetrics().height();

  auto cnt = characters.count();

  double span = 0.6;
  auto perOne = span / cnt;
  for (int i = 0; i < cnt; i++) {
      auto ratio = 0.5 + (1.0 - span - 0.5) / 2.0 + perOne / 2.0 + perOne * i;
      auto point = path.pointAtPercent(ratio <= 1.0 ? ratio : ratio - 1.0);
      //        p->drawLine(QPointF(), point); // 辅助观察线

      p->save();
      p->translate(point);    // 当前坐标系移动到椭圆上的这一点上
      p->rotate(ratio * 360 - 270); // 让字跟到旋转
      p->setPen(Qt::red);
      p->drawText(QPointF(-ww / 2.0, hh * 1.2), characters.at(i));  // 微调左右
      p->restore();
  }

  p->restore();
#endif

  /******************绘制下边字**********************/
  p->save();
  p->rotate(180);
  //    test();

  const QString numbers("1122334455");

  // 首先确定字体大小，字体大小决定了绘制的弧长
  QFont font2 = p->font();
  font2.setPixelSize(minorAxisLen / 8.0);
  p->setFont(font2);
  auto ww2 = p->fontMetrics().width(numbers.at(0));
  auto hh2 = p->fontMetrics().height();

  auto cnt2 = numbers.count();

  double span2 = 0.3;
  auto perOne2 = span2 / cnt2;
  for (int i = 0; i < cnt2; i++) {
      auto ratio2 =  1.0 - ((1.0 - span2 - 0.5) / 2.0 + perOne2 / 2.0 + perOne2 * i);
      auto point2 = path.pointAtPercent(ratio2 >= 0 ? ratio2 : 1.0 - ratio2);
      //        p->drawLine(QPointF(), point2); // 辅助观察线

      p->save();
      p->translate(point2);    // 当前坐标系移动到椭圆上的这一点上
      p->rotate(ratio2 * 360 + 270); // 让字跟到旋转
      p->setPen(Qt::red);
      p->drawText(QPointF(-ww2 / 2.0, -hh2 / 4.0), numbers.at(i));  // 微调左右
      p->restore();
  }

  p->restore();

  p->restore();   // first paint->save()
}
// clang-format on
