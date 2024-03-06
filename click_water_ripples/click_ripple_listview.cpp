#include "click_ripple_listview.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QVariantAnimation>

ListItemDelegae::ListItemDelegae(QObject *parent)
    : QStyledItemDelegate(parent), anime(new QVariantAnimation(parent)) {
  connect(anime, &QVariantAnimation::valueChanged, this,
          [this](const QVariant &value) {
            radius = value.toDouble();
            emit updateRect(clickedRect);
          });
  connect(anime, &QVariantAnimation::finished, this, [this]() {
    radius = 0;
    emit updateRect(clickedRect);
  });
}

void ListItemDelegae::paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const {
  if (index == clickedIndex) {
    QPainterPath path;

    QRectF rect(0, 0, radius, radius);
    rect.moveCenter(clickedPos);
    path.addEllipse(rect);

    QLinearGradient linearGrad(rect.topLeft(), rect.bottomRight());
    linearGrad.setColorAt(0, QColor("#82fcc5e4"));
    linearGrad.setColorAt(0.15, QColor("#82fda34b"));
    linearGrad.setColorAt(0.35, QColor("#82ff7882"));
    linearGrad.setColorAt(0.52, QColor("#82c8699e"));
    linearGrad.setColorAt(0.71, QColor("#827046aa"));
    linearGrad.setColorAt(0.87, QColor("#820c1db8"));
    linearGrad.setColorAt(1, QColor("#82020f75"));

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setClipRect(option.rect);
    painter->fillPath(path, linearGrad);
    painter->restore();
  }
  QStyledItemDelegate::paint(painter, option, index);
}

QSize ListItemDelegae::sizeHint(const QStyleOptionViewItem &option,
                                const QModelIndex &index) const {
  auto size = QStyledItemDelegate::sizeHint(option, index);
  size.rheight() = 70;
  return size;
}

bool ListItemDelegae::editorEvent(QEvent *event, QAbstractItemModel *model,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) {
  if (event->type() == QEvent::MouseButtonPress) {
    auto mouseEvent = static_cast<QMouseEvent *>(event);
    clickedPos = mouseEvent->pos();
    clickedIndex = index;
    clickedRect = option.rect;
    anime->setKeyValueAt(0, 10);
    anime->setKeyValueAt(1.0,
                         2 * qMax(option.rect.width(), option.rect.height()));
    anime->setDuration(400);
    anime->setEasingCurve(QEasingCurve::InCubic);
    anime->start();
  }
  return QStyledItemDelegate::editorEvent(event, model, option, index);
}

TestListView::TestListView(QWidget *parent)
    : delegate(new ListItemDelegae(this)), model(new QStandardItemModel(this)) {
  setItemDelegate(delegate);
  connect(delegate, &ListItemDelegae::updateRect, this,
          [this](const QRect &r) { this->viewport()->update(r); });
  setModel(model);

  for (int i = 0; i < 5; i++) {
    model->appendRow(
        new QStandardItem(QIcon(style()->standardIcon(QStyle::SP_ComputerIcon)),
                          "Test" + QString(std::to_string(i).c_str())));
  }
}
