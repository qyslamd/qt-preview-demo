#include "add_modify_cfg_dialog.h"
#include "ui_add_modify_cfg_dialog.h"
#include "existed_cfg_dialog.h"

#include <QFileDialog>

AddModifyCfgDialog::AddModifyCfgDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddModifyCfgDialog)
{
    ui->setupUi(this);
    init();
}

AddModifyCfgDialog::~AddModifyCfgDialog()
{
    delete ui;
}

void AddModifyCfgDialog::setTitle(Mode mode, const QString &title)
{
    mode_ = mode;
    ui->groupBoxRight->setTitle(title);

    if(mode_ == Add) {
        static const QString title("\346\267\273\345\212\240");
        ui->btnAddModify->setText(title);
        setWindowTitle(title);
    } else {
         static const QString title("\344\277\256\346\224\271");
         ui->btnAddModify->setText(title);
         setWindowTitle(title);
    }
}

void AddModifyCfgDialog::setCfg(const ProcessConfiguration &cfg)
{
    cfg_ = cfg;

    ui->lineEditAlias->setText(cfg.alias);
    ui->lineEditProcessName->setText(cfg.process_name);
    ui->lineEditProcessPath->setText(cfg.process_path);
    ui->lineEditWndTitle->setText(cfg.window_title);
    ui->lineEditClassName->setText(cfg.wnd_class_name);
    ui->checkBoxRestart->setChecked(cfg.kill_and_start);
    ui->textEditProcessParam->setPlainText(cfg.process_arguments.join(' '));
    ui->spinBoxWatingTime->setValue(cfg.waiting_time);
}

void AddModifyCfgDialog::init()
{
    // 别名
    connect(ui->lineEditAlias, &QLineEdit::textChanged, this, [this](const QString &)
    {
        checkCanAddOrModify();
    });
    // 进程名称
    connect(ui->lineEditProcessName, &QLineEdit::textChanged, this, [this](const QString &)
    {
        checkCanAddOrModify();
    });
    // 进程路径
    connect(ui->lineEditProcessPath, &QLineEdit::textChanged, this, [this](const QString &)
    {
        checkCanAddOrModify();
    });
    // 等待时间
    connect(ui->spinBoxWatingTime, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int )
    {
        checkCanAddOrModify();
    });
    // 参数列表
    connect(ui->textEditProcessParam, &QTextEdit::textChanged, this, &AddModifyCfgDialog::checkCanAddOrModify);
    // 窗口名称
    connect(ui->lineEditWndTitle, &QLineEdit::textChanged, this, [this](const QString &)
    {
        checkCanAddOrModify();
    });
    // 类名
    connect(ui->lineEditClassName, &QLineEdit::textChanged, this, [this](const QString &)
    {
        checkCanAddOrModify();
    });
    // 杀掉进程再启动
    connect(ui->checkBoxRestart, &QAbstractButton::toggled, this, [this](bool )
    {
        checkCanAddOrModify();
    });

    ui->btnAddModify->setEnabled(false);

    connect(ui->btnChoosePath, &QToolButton::clicked, this, [this]{
        auto path = QFileDialog::getOpenFileName(this,
                                                 "Choose a executable file",
                                                 QApplication::applicationDirPath(),
                                                 "Executable file(*.exe);;Batch Files(*.cmd*.bat);;All files(*.*)");
        if (!path.trimmed().isEmpty()) ui->lineEditProcessPath->setText(path.trimmed());
    });

    connect(ui->btnCancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(ui->btnAddModify, &QPushButton::clicked, this, &AddModifyCfgDialog::onBtnAddModifyClicked);
}

void AddModifyCfgDialog::checkCanAddOrModify()
{
    if (Add == mode_) {
        auto enabled = !ui->lineEditProcessName->text().trimmed().isEmpty();
        enabled = enabled && !ui->lineEditProcessPath->text().trimmed().isEmpty();
        enabled = enabled && ui->spinBoxWatingTime->value() > 0;

        ui->btnAddModify->setEnabled(enabled);
    } else {
        auto alias = ui->lineEditAlias->text().trimmed();
        auto processName = ui->lineEditProcessName->text().trimmed();
        auto processPath = ui->lineEditProcessPath->text().trimmed();
        auto waitTime = ui->spinBoxWatingTime->value();
        auto arguments = GetArgumentsOrdered(ui->textEditProcessParam->toPlainText().trimmed());
        auto wndTitle = ui->lineEditWndTitle->text().trimmed();
        auto className = ui->lineEditClassName->text().trimmed();
        auto killStart = ui->checkBoxRestart->isChecked();

        bool equals = cfgEquals({cfg_.id,
                                 alias,
                                 processName,
                                 processPath,
                                 arguments,
                                 wndTitle,
                                 className,
                                 waitTime,
                                 killStart}, cfg_);
        ui->btnAddModify->setEnabled(!equals);
    }
}

void AddModifyCfgDialog::onBtnAddModifyClicked()
{
    auto alias = ui->lineEditAlias->text().trimmed();
    auto processName = ui->lineEditProcessName->text().trimmed();
    auto processPath = ui->lineEditProcessPath->text().trimmed();
    auto waitTime = ui->spinBoxWatingTime->value();
    auto arguments = GetArgumentsOrdered(ui->textEditProcessParam->toPlainText().trimmed());
    auto wndTitle = ui->lineEditWndTitle->text().trimmed();
    auto className = ui->lineEditClassName->text().trimmed();
    auto killStart = ui->checkBoxRestart->isChecked();

    if (mode_ == Add) {
        auto id = ExistedCfgDialog::makeId();
        qInfo() << __FUNCTION__ << id;
        ExistedCfgDialog::AddProcessCfg({id,
                                         alias,
                                         processName,
                                         processPath,
                                         arguments,
                                         wndTitle,
                                         className,
                                         waitTime,
                                         killStart});
    } else {
        ExistedCfgDialog::ModifyProcessCfg({cfg_.id,
                                            alias,
                                            processName,
                                            processPath,
                                            arguments,
                                            wndTitle,
                                            className,
                                            waitTime,
                                            killStart});
    }

    this->accept();
}
