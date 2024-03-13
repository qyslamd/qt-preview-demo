#include "existed_cfg_dialog.h"
#include "ui_existed_cfg_dialog.h"

#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QStandardItem>

#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QFileIconProvider>
#include <QDesktopServices>
#include <QTimer>

#include "add_modify_cfg_dialog.h"

static auto CfgFile{"processConfig.json"};

ExistedCfgDialog::ExistedCfgDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExistedCfgDialog)
{
    ui->setupUi(this);
    initUi();
}

ExistedCfgDialog::~ExistedCfgDialog()
{
    delete ui;
}

int ExistedCfgDialog::makeId()
{
    auto path = QApplication::applicationDirPath();
    if (path.contains("/")) {
        path.append("/config/").append(CfgFile);
    } else {
        path.append("\\config\\").append(CfgFile);
    }

    if (!QFileInfo::exists(path)) {
        qWarning() << __FUNCTION__ << "Process configuration file not existed!";
        return -1;
    }

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << __FUNCTION__ << "Open file failed!";
        return -1;
    }
    QJsonParseError jpErr;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll(), &jpErr);
    file.close();

    if(jpErr.error != QJsonParseError::NoError) {
        qWarning() << __FUNCTION__ << "Json parse error:"<<jpErr.errorString();
        return -1;
    }
    auto rootObj = jsonDoc.object();
    auto processArray = rootObj.value("process").toArray();

    QList<int> idList;
    for (auto it = processArray.begin(); it != processArray.end(); ++it) {
        auto obj = it->toObject();
        auto id = obj.value("id").toInt();
        idList << id;
    }

    std::sort(idList.begin(), idList.end(), [](int a, int b)->bool{return a < b;});

    return idList.last() + 1;
}

void ExistedCfgDialog::ModifyProcessCfg(const ProcessConfiguration &cfg)
{
    auto path = QApplication::applicationDirPath();
    if (path.contains("/")) {
        path.append("/config/").append(CfgFile);
    } else {
        path.append("\\config\\").append(CfgFile);
    }

    if (!QFileInfo::exists(path)) {
        qWarning() << __FUNCTION__ << "Process configuration file not existed!";
        return ;
    }

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << __FUNCTION__ << "Open file failed!";
        return;
    }
    QJsonParseError jpErr;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll(), &jpErr);
    file.close();

    if(jpErr.error != QJsonParseError::NoError) {
        qWarning() << __FUNCTION__ << "Json parse error:"<<jpErr.errorString();
        return;
    }
    auto rootObj = jsonDoc.object();
    auto processArray = rootObj.value("process").toArray();

    for (auto it = processArray.begin(); it != processArray.end(); ++it) {
        auto obj = it->toObject();
        auto id = obj.value("id").toInt();
        if (id == cfg.id) {
            QJsonObject newObj{
                {"id",cfg.id},
                {"alias", cfg.alias},
                {"processName", cfg.process_name},
                {"processPath", cfg.process_path},
                {"processArguments", QJsonArray::fromStringList(cfg.process_arguments)},
                {"windowTitle", cfg.window_title},
                {"className", cfg.wnd_class_name},
                {"waitTime", cfg.waiting_time},
                {"killAndStart", cfg.kill_and_start}
            };
            *it = newObj;
        }
    }
    // 替换
    rootObj.insert("process", processArray);
    // 替换
    jsonDoc.setObject(rootObj);

    QFile file2(path);
    if (file2.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file2.write(jsonDoc.toJson());
    }
}

void ExistedCfgDialog::DeleteProcessCfg(const int id)
{
    auto path = QApplication::applicationDirPath();
    if (path.contains("/")) {
        path.append("/config/").append(CfgFile);
    } else {
        path.append("\\config\\").append(CfgFile);
    }

    if (!QFileInfo::exists(path)) {
        qWarning() << __FUNCTION__ << "Process configuration file not existed!";
        return ;
    }

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << __FUNCTION__ << "Open file failed!";
        return;
    }
    QJsonParseError jpErr;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll(), &jpErr);
    file.close();

    if(jpErr.error != QJsonParseError::NoError) {
        qWarning() << __FUNCTION__ << "Json parse error:"<<jpErr.errorString();
        return;
    }
    auto rootObj = jsonDoc.object();
    auto processArray = rootObj.value("process").toArray();

    for (auto it = processArray.begin(); it != processArray.end();) {
        auto obj = it->toObject();
        if (obj.value("id").toInt() == id) {
            processArray.erase(it++);
        } else {
            ++it;
        }
    }
    // 替换
    rootObj.insert("process", processArray);
    // 替换
    jsonDoc.setObject(rootObj);

    QFile file2(path);
    if (file2.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file2.write(jsonDoc.toJson());
    }
}

void ExistedCfgDialog::AddProcessCfg(const ProcessConfiguration &cfg)
{
    auto path = QApplication::applicationDirPath();
    if (path.contains("/")) {
        path.append("/config/").append(CfgFile);
    } else {
        path.append("\\config\\").append(CfgFile);
    }

    if (!QFileInfo::exists(path)) {
        qWarning() << __FUNCTION__ << "Process configuration file not existed!";
        return ;
    }

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << __FUNCTION__ << "Open file failed!";
        return;
    }
    QJsonParseError jpErr;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll(), &jpErr);
    file.close();

    if(jpErr.error != QJsonParseError::NoError) {
        qWarning() << __FUNCTION__ << "Json parse error:"<<jpErr.errorString();
        return;
    }
    auto rootObj = jsonDoc.object();
    auto processArray = rootObj.value("process").toArray();

    QJsonObject newObj{
        {"id",cfg.id},
        {"alias", cfg.alias},
        {"processName", cfg.process_name},
        {"processPath", cfg.process_path},
        {"processArguments", QJsonArray::fromStringList(cfg.process_arguments)},
        {"windowTitle", cfg.window_title},
        {"className", cfg.wnd_class_name},
        {"waitTime", cfg.waiting_time},
        {"killAndStart", cfg.kill_and_start}
    };
    // 追加
    processArray.append(newObj);
    // 替换
    rootObj.insert("process", processArray);
    // 替换
    jsonDoc.setObject(rootObj);

    QFile file2(path);
    if (file2.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file2.write(jsonDoc.toJson());
    }
}

void ExistedCfgDialog::showEvent(QShowEvent *event)
{
    // 应对从文件更改，选择在showEvent加载
    loadExistingCfg();
    QDialog::showEvent(event);
}

void ExistedCfgDialog::initUi()
{
    list_model_ = new QStandardItemModel(this);
    ui->listView->setModel(list_model_);
    ui->listView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->listView, &QAbstractItemView::clicked, this, &ExistedCfgDialog::onListViewItemClicked);

    // 刷新配置
    connect(ui->btnRefreshCfgFilePath, &QPushButton::clicked, this, [this]{
        list_model_->clear();
        QTimer::singleShot(500, this, &ExistedCfgDialog::loadExistingCfg);
    });

    // 打开配置文件所在路径
    connect(ui->btnOpenCfgFilePath, &QPushButton::clicked, this, [this]
    {
        QFileInfo fInfo(cfg_file_path_);
        QDesktopServices::openUrl(fInfo.absoluteDir().absolutePath());
    });

    connect(ui->btnAdd2StartPage, &QPushButton::clicked, this, [this]{
        auto index = ui->listView->currentIndex();
        if(!index.isValid()) {
            return;
        }
        emit add2StartPage(index.data(Qt::UserRole + 1).value<ProcessConfiguration>(), ui->checkBoxStartEmbeded->isChecked());
        this->accept();
    });
    ui->labelJsonErr->setVisible(false);

    connect(ui->btnAddProcessCfg, &QPushButton::clicked, this, [this]{
        AddModifyCfgDialog addDlg(this);
        addDlg.setTitle(AddModifyCfgDialog::Add, "\346\267\273\345\212\240\345\272\224\347\224\250\351\205\215\347\275\256");
        auto ret = addDlg.exec();
        if(ret == QDialog::Accepted) {
            loadExistingCfg();
        }
    });

    connect(ui->btnModify, &QToolButton::clicked, this, [this]{
        AddModifyCfgDialog addDlg(this);
        addDlg.setTitle(AddModifyCfgDialog::Modify, "\344\277\256\346\224\271\345\272\224\347\224\250\351\205\215\347\275\256");
        addDlg.setCfg(ui->listView->currentIndex().data(Qt::UserRole + 1).value<ProcessConfiguration>());
        auto ret = addDlg.exec();
        if(ret == QDialog::Accepted) {
            loadExistingCfg();
        }
    });

    connect(ui->btnDelete, &QToolButton::clicked, this, [this] {
        auto data =ui->listView->currentIndex().data(Qt::UserRole + 1).value<ProcessConfiguration>();
        DeleteProcessCfg(data.id);

        loadExistingCfg();
    });
}

void ExistedCfgDialog::loadExistingCfg()
{
    cfg_file_path_ = QApplication::applicationDirPath();
    if (cfg_file_path_.contains("/")) {
        cfg_file_path_.append("/config/").append(CfgFile);
    } else {
        cfg_file_path_.append("\\config\\").append(CfgFile);
    }

    if (!QFileInfo::exists(cfg_file_path_)) {
        QFile file(cfg_file_path_);
        auto status = file.open(QIODevice::ReadWrite);

        if(status) {
            auto jsonStr= R"json({
                                  "process": [
                                      {
                                          "alias": "Notepad++",
                                          "className": "Notepad++",
                                          "id": 1,
                                          "killAndStart": true,
                                          "processArguments": [
                                          ],
                                          "processName": "notepad++.exe",
                                          "processPath": "C:/Program Files (x86)/Notepad++/notepad++.exe",
                                          "waitTime": 5,
                                          "windowTitle": ""
                                      }
                                  ]
                                })json";
            file.write(jsonStr);
            file.close();
        }
    }

    QFile file(cfg_file_path_);
    if (!file.open(QIODevice::ReadOnly)) {
        handleErr(true, "打开Json文件失败");
        qWarning() << __FUNCTION__ << "Open file failed!";
        return;
    }
    QJsonParseError jpErr;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll(), &jpErr);
    file.close();

    if(jpErr.error != QJsonParseError::NoError) {
        handleErr(true, "解析Json文件出错，错误信息：" + jpErr.errorString());
        qWarning() << __FUNCTION__ << "Json parse error:"<<jpErr.errorString();

        return;
    }

    handleErr(false);

    auto rootObj = jsonDoc.object();
    auto processArray = rootObj.value("process").toArray();

    list_model_->clear();
    for (auto it = processArray.begin(); it != processArray.end(); ++it) {
        auto obj = it->toObject();
        auto id = obj.value("id").toInt();
        auto alias = obj.value("alias").toString();
        auto processName = obj.value("processName").toString();
        auto processPath = obj.value("processPath").toString();
        QStringList paras;
        auto argsArray = obj.value("processArguments").toArray();
        for (auto item : argsArray) {
            paras << item.toString();
        }
        auto windowTitle = obj.value("windowTitle").toString();
        auto className = obj.value("className").toString();
        auto waitTime = obj.value("waitTime").toInt();
        auto killAndStart = obj.value("killAndStart").toBool();

        QFileIconProvider provider;
        auto title = alias.isEmpty() ? processName : alias;
        auto item = new QStandardItem(provider.icon(QFileInfo(processPath)), alias);
        item->setData(QVariant::fromValue(ProcessConfiguration{
                                              id,
                                              alias,
                                              processName,
                                              processPath,
                                              paras,
                                              windowTitle,
                                              className,
                                              waitTime,
                                              killAndStart
                                          }), Qt::UserRole + 1);
        if (!QFileInfo::exists(processPath)) {
            item->setData(title + " [Invalid]", Qt::DisplayRole);
            item->setData(QColor(Qt::red), Qt::TextColorRole);
        }
        list_model_->appendRow(item);
    }

    auto item = list_model_->item(0, 0);
    auto index = list_model_->indexFromItem(item);
    if(index.isValid()) {
        onListViewItemClicked(index);
    }
}

void ExistedCfgDialog::handleErr(bool err, const QString &errString)
{
    if (err) {
        ui->labelJsonErr->setText(errString);
        ui->labelJsonErr->setVisible(true);
        ui->btnAdd2StartPage->setEnabled(false);
        ui->checkBoxStartEmbeded->setEnabled(false);
    } else {
        ui->labelJsonErr->setText("");
        ui->labelJsonErr->setVisible(false);
        ui->btnAdd2StartPage->setEnabled(true);
        ui->checkBoxStartEmbeded->setEnabled(true);
    }
}

void ExistedCfgDialog::onListViewItemClicked(const QModelIndex &index)
{
    auto data = index.data(Qt::UserRole + 1);
    auto cfgData = data.value<ProcessConfiguration>();

    ui->labelAlias->setText(cfgData.alias);
    ui->labelProcessName->setText(cfgData.process_name);
    ui->labelProcessPath->setText(cfgData.process_path);
    ui->spinBoxWatingTime->setValue(cfgData.waiting_time);
    ui->textEditProcessParam->setText(cfgData.process_arguments.join(' '));
    ui->labelWndTitle->setText(cfgData.window_title);
    ui->labelClassName->setText(cfgData.wnd_class_name);
    ui->checkBoxRestart->setChecked(cfgData.kill_and_start);
}
