#include "widget.h"

#include <QAction>
#include <QByteArray>
#include <QContextMenuEvent>
#include <QDateTime>
#include <QDir>
#include <QElapsedTimer>
#include <QEventLoop>
#include <QFile>
#include <QFileDialog>
#include <QFileIconProvider>
#include <QFileInfo>
#include <QFlags>
#include <QMenu>
#include <QMessageBox>
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QProcess>
#include <QStandardItemModel>
#include <QSvgRenderer>
#include <QTabBar>
#include <QTimer>
#include <QWindow>
#include <QtDebug>
#include <QtMath>
#include <complex>
#include <future>
#include <iostream>
#include <thread>

#include "app_settings.h"
#include "existed_cfg_dialog.h"
#include "ui_widget.h"
#include "windows_helper.hpp"

static auto killProcessFun = [](HWND hwnd) -> bool {
  unsigned long processId = 0;
  if (::GetWindowThreadProcessId(hwnd, &processId)) {
    auto ret = win_helper::TerminateApp(processId, 5000);
    if (ret != TA_FAILED) {
      qInfo() << "Process terminated!" << ret;
      return true;
    }
  }
  return false;
};

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
  ui->setupUi(this);
  init();
  initEmbedSettingsUi();
  initOtherTests();
}

Widget::~Widget() { delete ui; }

bool Widget::eventFilter(QObject *obj, QEvent *event) {
  if (obj == ui->tabWidget->tabBar()) {
    if (event->type() == QEvent::ContextMenu) {
      if (!tabbar_menu_) {
        tabbar_menu_ = new QMenu(ui->tabWidget->tabBar());
        tabbar_menu_->addAction(
            QIcon(":/resource/imgs/shutdown_96px.png"),
            "\346\235\200\346\216\211\350\277\233\347\250\213", this,
            &Widget::onKillActionClicked);
        tabbar_menu_->addAction(QIcon(":/resource/imgs/eject_96px.png"),
                                "\345\274\271\345\207\272", this,
                                &Widget::onPopupActionClicked);
      }

      auto contextEvent = static_cast<QContextMenuEvent *>(event);
      tabbar_menu_trigger_index_ =
          ui->tabWidget->tabBar()->tabAt(contextEvent->pos());
      tabbar_menu_->exec(contextEvent->globalPos());
    }
  }
  return QWidget::eventFilter(obj, event);
}

void Widget::closeEvent(QCloseEvent *event) {
  AppSettings::setWindowGeometry(saveGeometry());

  QWidget::closeEvent(event);
}

void Widget::init() {
  setWindowIcon(QIcon(":/resource/imgs/test_tube_96px.png"));

  auto geo = AppSettings::windowGeometry();
  if (geo.isEmpty() || !restoreGeometry(geo)) {
    resize(1000, 600);
  }

  // splitterTabWidget
  connect(ui->splitterTabWidget, &QSplitter::splitterMoved, this,
          [this](int, int) {
            AppSettings::saveTabwidgetSplitterState(
                ui->splitterTabWidget->saveState());
          });
  ui->splitterTabWidget->restoreState(AppSettings::tabwidgetSplitterState());

  // splitter central
  connect(ui->splitter, &QSplitter::splitterMoved, this, [this](int, int) {
    AppSettings::saveCentralSplitterState(ui->splitter->saveState());
  });
  ui->splitter->restoreState(AppSettings::centralSplitterState());

  // tabbar
  ui->tabWidget->tabBar()->installEventFilter(this);

  // configured dialog
  connect(ui->btnExistedCfg, &QToolButton::clicked, this, [this] {
    if (!existed_cfg_dialog_) {
      existed_cfg_dialog_ = new ExistedCfgDialog(this);
      connect(existed_cfg_dialog_, &ExistedCfgDialog::add2StartPage, this,
              [this](const ProcessConfiguration &info, bool start_embed) {
                if (ui->groupBoxEmbedCfg->isEnabled()) {
                  ui->lineEditAppAlias->setText(info.alias);
                  ui->lineEditProcessName->setText(info.process_name);
                  ui->lineEditProcessPath->setText(info.process_path);
                  ui->textEditProcessParam->setPlainText(
                      info.process_arguments.join(' '));
                  ui->spinBoxWatingTime->setValue(info.waiting_time);
                  ui->lineEditWndTitle->setText(info.window_title);
                  ui->lineEditClassName->setText(info.wnd_class_name);
                  ui->checkBoxRestart->setChecked(info.kill_and_start);

                  if (start_embed) {
                    QTimer::singleShot(10, this,
                                       &Widget::onBtnStartAndEmbededClicked);
                  }
                }
              });
    }

    existed_cfg_dialog_->exec();
  });
  auto labelMenuHint = new QLabel;
  labelMenuHint->setStyleSheet("color:red;");
  labelMenuHint->setText(
      "\345\234\250\346\240\207\347\255\276\351\241\265\344\270\212\347\202\271"
      "\345\207\273\351\274\240\346\240\207\345\217\263\351\224\256\357\274\214"
      "\345\217\257\344\273\245\345\274\271\345\207\272\350\217\234\345\215"
      "\225");
  ui->tabWidget->setCornerWidget(labelMenuHint, Qt::TopRightCorner);

  // 时间日期
  auto timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, [this] {
    ui->labelDateTime->setText(QDateTime::currentDateTime().toString());
  });
  timer->start(1000);
  // tab关闭
  ui->tabWidget->setTabsClosable(true);
  connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this,
          &Widget::onTabCloseRequested);

  // 日志清理
  connect(ui->btnClearLog, &QToolButton::clicked, ui->textBrowser,
          &QTextBrowser::clear);

  ui->labelHint->setVisible(false);
}

void Widget::initEmbedSettingsUi() {
  ui->btnStartAndEmbeded->setEnabled(false);
  ui->btnStartNotEmbeded->setEnabled(false);

  // 程序名称
  connect(ui->lineEditProcessName, &QLineEdit::textChanged, this,
          [this](const QString &) {
            auto empty = ui->lineEditProcessName->text().trimmed().isEmpty();
            ui->btnStartAndEmbeded->setEnabled(!empty);
            ui->btnStartNotEmbeded->setEnabled(!empty);
          });

  // 可执行体路径
  connect(ui->lineEditProcessPath, &QLineEdit::textChanged, this,
          [this](const QString &) {
            auto path = ui->lineEditProcessPath->text().trimmed();
            auto enable = !path.isEmpty();

            QFileInfo info(path);
            enable = enable && info.exists();

            ui->btnStartAndEmbeded->setEnabled(enable);
            ui->btnStartNotEmbeded->setEnabled(enable);
          });
  connect(ui->btnChoosePath, &QToolButton::clicked, this, [this] {
    auto path = QFileDialog::getOpenFileName(
        this, "Choose a executable file", QApplication::applicationDirPath(),
        "Executable file(*.exe);;Batch Files(*.cmd*.bat);;All files(*.*)");
    if (!path.trimmed().isEmpty())
      ui->lineEditProcessPath->setText(path.trimmed());
  });

  connect(ui->btnStartAndEmbeded, &QPushButton::clicked, this,
          &Widget::onBtnStartAndEmbededClicked);
  connect(ui->btnStartNotEmbeded, &QPushButton::clicked, this,
          &Widget::onBtnStartNotEmbededClicked);
}

void Widget::initOtherTests() {
  auto guoGaiWidget = new QWidget(this);
  guoGaiWidget->winId();  // 调用这个函数将会被分配原生的窗口句柄！！！

  guoGaiWidget->setWindowFlag(Qt::FramelessWindowHint);
  guoGaiWidget->hide();
  guoGaiWidget->setStyleSheet("background-color:red;");

  auto hideWidget = [this](QWidget *target, QWidget *guogai) {
    // 借助屏幕坐标完成坐标转换。
    auto posToGlobal = target->mapToGlobal(target->rect().topLeft());
    // 把全局坐标转换成为锅盖的本地坐标
    auto posToGuoGai = this->mapFromGlobal(posToGlobal);

    guogai->resize(target->size());
    guogai->move(posToGuoGai);
    guogai->raise();
    guogai->show();

    //
    HWND handle = reinterpret_cast<HWND>(guogai->winId());
    ::SetWindowPos(handle, HWND_TOPMOST, 0, 0, 0, 0,
                   SWP_NOMOVE | SWP_NOREDRAW | SWP_NOSIZE);

    //        handle = reinterpret_cast<HWND>(target->winId());
    //        ::SetWindowPos(handle, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE |
    //        SWP_NOREDRAW | SWP_NOSIZE);
  };

  connect(ui->btnGuogai, &QPushButton::clicked, this, [=]() {
    hideWidget(ui->tabWidget, guoGaiWidget);

    ui->btnGuogai->setEnabled(false);
  });
}

void Widget::removeIfProcessExisted(const QString &processName) {
  auto isProcessExisted = [](QTabWidget *tab, const QString processName,
                             int &index) -> bool {
    for (auto i = 0; i < tab->count(); i++) {
      auto data = tab->tabBar()->tabData(i).value<EmbededInfo>();
      if (processName.compare(data.cfg.process_name, Qt::CaseInsensitive) ==
          0) {
        index = i;
        return true;
      }
    }
    return false;
  };

  int index = -1;
  while (isProcessExisted(ui->tabWidget, processName, index)) {
    showLog("<Process Kill> process is in tab:" + QString::number(index) +
            ".It will be removed!");
    // 移除
    auto data = ui->tabWidget->tabBar()->tabData(index).value<EmbededInfo>();
    auto qt_window = data.qt_widow;
    auto widget = ui->tabWidget->widget(index);
    ui->tabWidget->removeTab(index);
    qt_window->destroy();
    delete widget;
  }
}

void Widget::executeProcessKill(const QString &processName) {
  showLog("<Process Kill>  Execute kill process:" + processName);

  win_helper::windows_handle_data data{-1, NULL};
  if (win_helper::FindWinIdByProcessName(processName.toStdString(), data)) {
    showLog("<Process Kill> Find the process, executing kill....");
    if (auto hwnd = data.best_handle) {
      std::future<bool> future =
          std::async(std::launch::async, killProcessFun, hwnd);
      qInfo() << "waiting...";

      std::future_status status;
      do {
        status = future.wait_for(std::chrono::seconds(1));
        if (status == std::future_status::deferred) {
          qInfo() << "deferred";
        } else if (status == std::future_status::timeout) {
          qInfo() << "timeout";
        } else if (status == std::future_status::ready) {
          qInfo() << "ready!";
        }
        QApplication::processEvents(QEventLoop::AllEvents);
      } while (status != std::future_status::ready);

      auto ret = future.get();

      if (ret) {
        showLog("<Process Kill> kill process" + processName + "finished!");
        qInfo() << "niubi! kill process" << processName << "finished!";
      }
    }
  } else {
    showLog("<Process Kill> No prcocess named " + processName + "to kill");
    qInfo() << __FUNCTION__ << "No prcocess named " << processName;
  }
}

void Widget::killProcessAsync(WId hwnd) {}

void Widget::executeEmbed(HWND hwnd, const ProcessConfiguration &settings) {
  showLog("<Embed window> Now Embeding the window....");
  if (hwnd && ::IsWindow(hwnd)) {
    showLog("<Embed window> Find window successfully!");
    LONG gml_style = ::GetWindowLong(hwnd, -16);
    /*
     * 嵌入前：
     *      窗口样式： 0x14CF0000
     *      扩展样式： 0x00000100
     * */
    if (gml_style == 0x14CF0000) {
      qInfo() << "Before embed,window style equals!";
    } else {
      qInfo() << "Before embed,window style different!" << gml_style;
    }

    HMENU menu = ::GetMenu(hwnd);

    auto qt_window = QWindow::fromWinId((WId)hwnd);
    qInfo() << "Before embed,qt_window:" << qt_window;

    if (qt_window) {
      auto widget = QWidget::createWindowContainer(qt_window, ui->tabWidget,
                                                   Qt::FramelessWindowHint);
      auto title =
          settings.alias.isEmpty() ? settings.process_name : settings.alias;
      QFileIconProvider provider;
      auto index = ui->tabWidget->addTab(
          widget, provider.icon(QFileInfo(settings.process_path)), title);

      EmbededInfo info{settings, qt_window, gml_style, menu};
      ui->tabWidget->tabBar()->setTabData(index, QVariant::fromValue(info));
      ui->tabWidget->setCurrentIndex(index);
      /*
       * 嵌入后：
       *      窗口样式： 0x54000000
       *      扩展样式： 0x00000000
       * */
      showLog("<Embed window> Embed window successfully! tab index is: " +
              QString::number(index));
    }

    LONG gml_style2 = ::GetWindowLong(hwnd, -16);
    if (gml_style2 == 0x54000000) {
      qInfo() << "After embeded,window style equals!";
    } else {
      qInfo() << "After embeded,window style different!" << gml_style;
    }
  } else {
    showLog("<Embed window> Find window failed! Embed failed!");
  }
}

void Widget::executeUnEmbed(HWND hwnd, HMENU menu) {
  auto pos = ui->tabWidget->mapToGlobal(ui->tabWidget->rect().topLeft());
  auto size = ui->tabWidget->sizeHint();

  ::SetParent(hwnd, nullptr);
  ::SetWindowLong(hwnd, -16, 0x14CF0000 | WS_SYSMENU);
  ::SetWindowLong(hwnd, -20, 0x00000100);
  ::MoveWindow(hwnd, pos.x(), pos.y(), size.width(), size.height(), FALSE);
  ::ShowWindow(hwnd, SW_SHOW);

  if (menu) {
    ::SetMenu(hwnd, menu);
    ::DrawMenuBar(hwnd);
  }
}

void Widget::showLog(const QString &log) {
  if (ui->checkBoxShowLog->isChecked()) {
    ui->textBrowser->append(
        QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "-" +
        log);
  }
}

void Widget::onBtnStartAndEmbededClicked() {
  auto processName = ui->lineEditProcessName->text().trimmed();
  if (processName.isEmpty()) {
    ui->labelHint->setText("the process name is empty!");
    ui->labelHint->setVisible(true);
    return;
  }
  auto processPath = ui->lineEditProcessPath->text().trimmed();
  if (processPath.isEmpty()) {
    ui->labelHint->setText("The process full path is empty!");
    ui->labelHint->setVisible(true);
    return;
  }

  ui->labelHint->setText("");
  ui->labelHint->setVisible(false);

  auto alias = ui->lineEditAppAlias->text().trimmed();
  auto waitingTime = ui->spinBoxWatingTime->value();
  auto arguments = ui->textEditProcessParam->toPlainText().trimmed();
  auto wndTitle = ui->lineEditWndTitle->text().trimmed();
  auto className = ui->lineEditClassName->text().trimmed();
  auto restart = ui->checkBoxRestart->isChecked();

  ProcessConfiguration settings{
      -1,       alias,     processName,        processPath, {},
      wndTitle, className, waitingTime * 1000, restart};

  win_helper::windows_handle_data data{-1, NULL};
  // 检查当前进程的执行情况
  if (restart) {
    // 如果进程在当前的tab页面中，执行移除
    removeIfProcessExisted(processName);
    executeProcessKill(processName);
    goto start_lbl;
  } else {
    if (win_helper::FindWinIdByProcessName(processName.toStdString(), data)) {
      executeEmbed(data.best_handle, settings);
      return;
    }
  }

start_lbl:
  // 启动应用
  auto argList = GetArguments(arguments);
  qInfo() << "Unordered parameters:" << argList;
  argList = GetArgumentsOrdered(arguments);
  qInfo() << "Ordered parameters:" << argList;

  QProcess process;
  process.setProgram(processPath);
  process.setArguments(argList);

  QEventLoop loop;
  process.startDetached(nullptr);
  showLog("<Start process> Starting process " + processPath +
          argList.join(' '));
  QTimer::singleShot(waitingTime * 1000, this, [&loop] { loop.quit(); });
  ui->groupBoxEmbedCfg->setEnabled(false);
  loop.exec();
  ui->groupBoxEmbedCfg->setEnabled(true);

  HWND handle = NULL;
  if (win_helper::FindWinIdByProcessName(processName.toStdString(), data)) {
    handle = data.best_handle;
    showLog(QString("<Start process> FindWinIdByProcessName:%1 succeed! window "
                    "handle is:")
                .arg(processName) +
            QString::number((WId)handle));
  } else {
    handle = win_helper::FindWinidByClassName(className, wndTitle);
    showLog(QString("<Start process> FindWinIdByProcessName:%1 failed! Use "
                    "FindWinidByClassName:%2")
                .arg(processName)
                .arg(wndTitle));
  }

  settings.process_arguments = argList;
  // 执行程序嵌入
  executeEmbed(handle, settings);
}

void Widget::onBtnStartNotEmbededClicked() {
  auto processName = ui->lineEditProcessName->text().trimmed();
  if (processName.isEmpty()) {
    ui->labelHint->setText("the process name is empty!");
    return;
  }
  auto processPath = ui->lineEditProcessPath->text().trimmed();
  if (processPath.isEmpty()) {
    ui->labelHint->setText("The process full path is empty!");
    return;
  }
  auto waitingTime = ui->spinBoxWatingTime->value();
  auto arguments = ui->textEditProcessParam->toPlainText().trimmed();
  auto wndTitle = ui->lineEditWndTitle->text().trimmed();
  auto className = ui->lineEditClassName->text().trimmed();
  auto restart = ui->checkBoxRestart->isChecked();

  win_helper::windows_handle_data data{-1, NULL};
  // 检查当前进程的执行情况
  if (restart) {
    executeProcessKill(processName);
    goto start_lbl2;
  } else {
    if (win_helper::FindWinIdByProcessName(processName.toStdString(), data)) {
      ::SetForegroundWindow(data.best_handle);
      return;
    }
  }
start_lbl2:
  // 启动应用
  auto argList = GetArguments(arguments);
  qInfo() << "Unordered parameters:" << argList;
  argList = GetArgumentsOrdered(arguments);
  qInfo() << "Ordered parameters:" << argList;

  QProcess process;
  process.setProgram(processPath);
  process.setArguments(argList);

  QEventLoop loop;
  process.startDetached(nullptr);
  showLog("<Process start> Starting process " + processPath);
  QTimer::singleShot(waitingTime * 1000, this, [&loop] { loop.quit(); });
  ui->groupBoxEmbedCfg->setEnabled(false);
  loop.exec();
  ui->groupBoxEmbedCfg->setEnabled(true);
}

void Widget::onKillActionClicked() {
  auto data = ui->tabWidget->tabBar()
                  ->tabData(tabbar_menu_trigger_index_)
                  .value<EmbededInfo>();
  showLog("<Process Kill> Start process kill..." + data.cfg.process_name);

  auto qt_window = data.qt_widow;
  auto id = qt_window->winId();

  std::future<bool> future =
      std::async(std::launch::async, killProcessFun, (HWND)id);
  showLog("<Process Kill> Starting process kill asynchronous!");

  std::future_status status;
  do {
    status = future.wait_for(std::chrono::seconds(1));
    if (status == std::future_status::deferred) {
      showLog("<Process Kill> std::future: deferred");
    } else if (status == std::future_status::timeout) {
      showLog("<Process Kill> std::future: timeout");
    } else if (status == std::future_status::ready) {
      showLog("<Process Kill> std::future: ready");
    }
    QApplication::processEvents(QEventLoop::AllEvents);
  } while (status != std::future_status::ready);

  auto ret = future.get();

  if (ret) {
    showLog(QString("<Process Kill> Kill process %1 successully!")
                .arg(data.cfg.process_name));

    auto widget = ui->tabWidget->widget(tabbar_menu_trigger_index_);
    ui->tabWidget->removeTab(tabbar_menu_trigger_index_);
    qt_window->destroy();
    delete widget;

    tabbar_menu_trigger_index_ = -1;
  }
}

void Widget::onPopupActionClicked() {
  qInfo() << __FUNCTION__ << tabbar_menu_trigger_index_;

  auto data = ui->tabWidget->tabBar()
                  ->tabData(tabbar_menu_trigger_index_)
                  .value<EmbededInfo>();
  auto menu = (HMENU)data.native_menu;
  auto qt_window = data.qt_widow;
  auto id = qt_window->winId();

  if (auto hwnd = (HWND)id) {
    auto pos = ui->tabWidget->mapToGlobal(ui->tabWidget->rect().topLeft());
    auto widget = ui->tabWidget->widget(tabbar_menu_trigger_index_);
    auto size = widget->size();
    ui->tabWidget->removeTab(tabbar_menu_trigger_index_);
    qt_window->destroy();
    delete widget;

    tabbar_menu_trigger_index_ = -1;

    ::SetParent(hwnd, nullptr);
    ::SetWindowLong(hwnd, -16, 0x14CF0000 | WS_SYSMENU);
    ::SetWindowLong(hwnd, -20, 0x00000100);
    ::MoveWindow(hwnd, pos.x(), pos.y(), size.width(), size.height(), FALSE);
    ::ShowWindow(hwnd, SW_SHOW);

    ::SetMenu(hwnd, menu);
    ::DrawMenuBar(hwnd);

    //
    ::UpdateWindow(hwnd);
  }
}

void Widget::onTabCloseRequested(int index) {
  auto data = ui->tabWidget->tabBar()->tabData(index).value<EmbededInfo>();

  if (QMessageBox::question(this, "Are you sure",
                            QString("Are you sure to close \"%1?\"")
                                .arg(data.cfg.process_name)) ==
      QMessageBox::No) {
    return;
  }

  auto qt_window = data.qt_widow;
  auto id = qt_window->winId();

  std::future<bool> future =
      std::async(std::launch::async, killProcessFun, (HWND)id);
  showLog("<Process Kill> Starting process kill asynchronous!");

  std::future_status status;
  do {
    status = future.wait_for(std::chrono::seconds(1));
    if (status == std::future_status::deferred) {
      showLog("<Process Kill> std::future: deferred");
    } else if (status == std::future_status::timeout) {
      showLog("<Process Kill> std::future: timeout");
    } else if (status == std::future_status::ready) {
      showLog("<Process Kill> std::future: ready");
    }
    QApplication::processEvents(QEventLoop::AllEvents);
  } while (status != std::future_status::ready);

  auto ret = future.get();

  if (ret) {
    auto widget = ui->tabWidget->widget(index);
    ui->tabWidget->removeTab(index);
    qt_window->destroy();
    delete widget;
  }
}
