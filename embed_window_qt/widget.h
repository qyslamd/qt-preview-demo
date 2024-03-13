#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <Windows.h>

#include "data_def.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class QMenu;
class ExistedCfgDialog;
class QStandardItemModel;
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
private:
    void init();
    void initEmbedSettingsUi();
    void initOtherTests();
private:
    void removeIfProcessExisted(const QString &processName);
    void executeProcessKill(const QString &processName);
    void killProcessAsync(WId hwnd);
    void executeEmbed(HWND hwnd, const ProcessConfiguration &settings);
    void executeUnEmbed(HWND hwnd, HMENU menu);
private:
    void showLog(const QString &log);
private slots:
    void onBtnStartAndEmbededClicked();
    void onBtnStartNotEmbededClicked();
    void onKillActionClicked();
    void onPopupActionClicked();
    void onTabCloseRequested(int index);
private:
    Ui::Widget *ui;
    ExistedCfgDialog *existed_cfg_dialog_ = nullptr;
    QMenu *tabbar_menu_ = nullptr;
    int tabbar_menu_trigger_index_ = -1;
};

#endif // WIDGET_H
