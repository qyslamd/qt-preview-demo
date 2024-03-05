#include "widget.h"

#include <QTimer>
#include <QtDebug>

#include "bagua.h"
#include "bagua_round.h"

Widget::Widget(QWidget *parent)
    : QTabWidget(parent),
      bagua_(new Bagua(this)),
      round_bagua_(new BaguaRound(this)) {
  addTab(bagua_, "Polygon Bagua");
  addTab(round_bagua_, "Round Bagua");
}
