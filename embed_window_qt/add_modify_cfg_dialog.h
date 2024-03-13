#ifndef ADD_MODIFY_CFG_DIALOG_H
#define ADD_MODIFY_CFG_DIALOG_H

#include <QDialog>

#include "data_def.h"

namespace Ui {
class AddModifyCfgDialog;
}

class AddModifyCfgDialog : public QDialog
{
    Q_OBJECT

public:
    enum Mode{
        Add,
        Modify
    };
    explicit AddModifyCfgDialog(QWidget *parent = nullptr);
    ~AddModifyCfgDialog();

    void setTitle(Mode mode, const QString &title);
    void setCfg(const ProcessConfiguration &cfg);
private:
    void init();
    void checkCanAddOrModify();

private slots:
    void onBtnAddModifyClicked();
private:
    Ui::AddModifyCfgDialog *ui;
    Mode mode_;

    ProcessConfiguration cfg_;
};

#endif // ADD_MODIFY_CFG_DIALOG_H
