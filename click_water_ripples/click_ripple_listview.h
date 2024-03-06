#pragma once

#include <QListView>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QWidget>

class QVariantAnimation;
class ListItemDelegae : public QStyledItemDelegate {
  Q_OBJECT
 public:
  ListItemDelegae(QObject *parent = nullptr);
  // QAbstractItemDelegate interface
 public:
  void paint(QPainter *painter, const QStyleOptionViewItem &option,
             const QModelIndex &index) const override;
  QSize sizeHint(const QStyleOptionViewItem &option,
                 const QModelIndex &index) const override;
  bool editorEvent(QEvent *event, QAbstractItemModel *model,
                   const QStyleOptionViewItem &option,
                   const QModelIndex &index) override;
 signals:
  void updateRect(const QRect &rect);

 private:
  QVariantAnimation *anime;
  double radius;
  QModelIndex clickedIndex;
  QPointF clickedPos;
  QRect clickedRect;
};

class TestListView : public QListView {
  Q_OBJECT
 public:
  TestListView(QWidget *parent = nullptr);

 private:
  ListItemDelegae *delegate;
  QStandardItemModel *model;
};
