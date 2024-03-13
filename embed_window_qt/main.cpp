#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    AllocConsole();
    freopen("CONOUT$","w+t",stdout);
    freopen("CONIN$","r+t",stdin);

    QApplication a(argc, argv);
    Widget w;
    w.show();
    auto ret = a.exec();
    FreeConsole();

    return ret;
}
