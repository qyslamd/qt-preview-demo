#ifndef EXISTED_CFG_DIALOG_H
#define EXISTED_CFG_DIALOG_H

#include <QDialog>

#include "data_def.h"

namespace Ui {
class ExistedCfgDialog;
}

class QStandardItemModel;
class QStyledItemDelegate;
class ExistedCfgDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExistedCfgDialog(QWidget *parent = nullptr);
    ~ExistedCfgDialog();

    static int makeId();
    static void AddProcessCfg(const ProcessConfiguration &cfg);
    static void ModifyProcessCfg(const ProcessConfiguration &cfg);
    static void DeleteProcessCfg(const int id);
signals:
    void add2StartPage(const ProcessConfiguration &info, bool start_embed);

protected:
    void showEvent(QShowEvent *event) override;
private:
    void initUi();
    void loadExistingCfg();
    void handleErr(bool err, const QString &errString = QString());

private slots:
    void onListViewItemClicked(const QModelIndex &index);
private:
    Ui::ExistedCfgDialog *ui;
    QStandardItemModel *list_model_ = nullptr;
    QString cfg_file_path_;
};

#endif // EXISTED_CFG_DIALOG_H
