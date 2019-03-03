/*
    Original copyright 2018 Benjamin Vedder benjamin@vedder.se and the VESC Tool project ( https://github.com/vedderb/vesc_tool )
    Now forked to:
    Danny Bokma github@diebie.nl

    This file is part of BMS Tool.

    DieBieMS Tool is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DieBieMS Tool is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"

namespace {

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    QString str;
    QString typeStr;
    bool isBad = false;

    switch (type) {
    case QtDebugMsg: typeStr = "DEBUG"; break;
        //    case QtInfoMsg: typeStr = "INFO"; break;
    case QtWarningMsg: typeStr = "WARNING"; isBad = true; break;
    case QtCriticalMsg: typeStr = "CRITICAL"; isBad = true; break;
    case QtFatalMsg: typeStr = "FATAL"; isBad = true; break;

    default:
        break;
    }

    str.sprintf("%s (%s:%u %s): %s", typeStr.toLocal8Bit().data(),
                context.file, context.line, context.function, localMsg.constData());

    if (PageDebugPrint::currentMsgHandler) {
        QString strTmp;
        if (isBad) {
            strTmp = "<font color=\"red\">" + str + "</font><br>";
        } else {
            strTmp = str + "<br>";
        }

        QMetaObject::invokeMethod(PageDebugPrint::currentMsgHandler, "printConsole",
                                  Qt::QueuedConnection, Q_ARG(QString, strTmp));
    }

    printf("%s\n", str.toLocal8Bit().data());
    fflush(stdout);
}
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QLoggingCategory::setFilterRules("qt.network.ssl.warning=false");

    mVersion = QString::number(DT_VERSION);
    mDieBieMS = new BMSInterface(this);
    mStatusInfoTime = 0;
    mStatusLabel = new QLabel(this);
    ui->statusBar->addPermanentWidget(mStatusLabel);
    mTimer = new QTimer(this);
    mKeyLeft = false;
    mKeyRight = false;
    mMcConfRead = false;

    connect(mTimer, SIGNAL(timeout()),this, SLOT(timerSlot()));
    connect(mDieBieMS, SIGNAL(statusMessage(QString,bool)),this, SLOT(showStatusInfo(QString,bool)));
    connect(mDieBieMS, SIGNAL(messageDialog(QString,QString,bool,bool)),this, SLOT(showMessageDialog(QString,QString,bool,bool)));
    connect(mDieBieMS, SIGNAL(serialPortNotWritable(QString)),this, SLOT(serialPortNotWritable(QString)));
    connect(mDieBieMS->commands(), SIGNAL(bmsConfigCheckResult(QStringList)),this, SLOT(bmsConfigCheckResult(QStringList)));
    connect(ui->actionAboutQt, SIGNAL(triggered(bool)),qApp, SLOT(aboutQt()));

    // Remove the menu with the option to hide the toolbar
    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    mDieBieMS->bmsConfig()->loadParamsXml("://res/parameters_bmsconfig.xml");
    mDieBieMS->infoConfig()->loadParamsXml("://res/info.xml");
    reloadPages();

    connect(mDieBieMS->bmsConfig(), SIGNAL(updated()),this, SLOT(bmsconfUpdated()));

    qInstallMessageHandler(myMessageOutput);

    mTimer->start(20);

    // Restore size and position
    if (mSettings.contains("mainwindow/size")) {
        resize(mSettings.value("mainwindow/size").toSize());
    }

    if (mSettings.contains("mainwindow/position")) {
        move(mSettings.value("mainwindow/position").toPoint());
    }

    if (mSettings.contains("mainwindow/maximized")) {
        bool maximized = mSettings.value("mainwindow/maximized").toBool();
        if (maximized) {
            showMaximized();
        }
    }

    mPageDebugPrint->printConsole("DieBieMSTool " + mVersion + " started<br>");
}

MainWindow::~MainWindow()
{
    // Save settings
    mSettings.setValue("version", mVersion);
    mSettings.setValue("mainwindow/position", pos());
    mSettings.setValue("mainwindow/maximized", isMaximized());

    if (!isMaximized()) {
        mSettings.setValue("mainwindow/size", size());
    }

    delete ui;
}

void MainWindow::timerSlot()
{
    // Update status label
    if (mStatusInfoTime) {
        mStatusInfoTime--;
        if (!mStatusInfoTime) {
            mStatusLabel->setStyleSheet(qApp->styleSheet());
        }
    } else {
        QString str = mDieBieMS->getConnectedPortName();
        if (str != mStatusLabel->text()) {
            mStatusLabel->setText(mDieBieMS->getConnectedPortName());
            static QString statusLast = "";
            if (str != statusLast) {
                mPageDebugPrint->printConsole("Status: " + str + "<br>");
                statusLast = str;
            }
        }
    }

    // CAN fwd
    if (ui->actionCanFwd->isChecked() != mDieBieMS->commands()->getSendCan()) {
        ui->actionCanFwd->setChecked(mDieBieMS->commands()->getSendCan());
    }

    // RT data only every 5 iterations
    if (ui->actionRtData->isChecked()) {
        static int values_cnt = 0;
        static int cells_cnt = 0;

        values_cnt++;
        if(values_cnt >= 5) {
            values_cnt = 0;
            mDieBieMS->commands()->getValues();
        }

        cells_cnt++;
        if(cells_cnt >= 20) {
            cells_cnt = 0;
            mDieBieMS->commands()->getCells();
        }
    }

    // Send alive command once every 10 iterations
    if (ui->actionSendAlive->isChecked()) {
        static int alive_cnt = 0;
        alive_cnt++;
        if (alive_cnt >= 10) {
            alive_cnt = 0;
            mDieBieMS->commands()->sendAlive();
        }
    }

    // Read configuration it isn't read since starting VESC Tool
    if (mDieBieMS->isPortConnected()) {
        static int conf_cnt = 0;
        conf_cnt++;
        if (conf_cnt >= 20) {
            conf_cnt = 0;
            if (!mMcConfRead) {
                mDieBieMS->commands()->getBMSconf();
            }
        }
    }

    // Disable all data streaming when uploading firmware
    if (mDieBieMS->commands()->getFirmwareUploadProgress() > 0.1) {
        ui->actionSendAlive->setChecked(false);
        ui->actionRtData->setChecked(false);
    }

    // Run startup checks
    static bool has_run_start_checks = false;
    if (!has_run_start_checks) {
        has_run_start_checks = true;
        checkUdev();
        Utility::checkVersion(mDieBieMS);
    }
}

void MainWindow::showStatusInfo(QString info, bool isGood)
{
    if (isGood) {
        mStatusLabel->setStyleSheet("QLabel { background-color : lightgreen; color : black; }");
        mPageDebugPrint->printConsole("Status: " + info + "<br>");
    } else {
        mStatusLabel->setStyleSheet("QLabel { background-color : red; color : black; }");
        mPageDebugPrint->printConsole("<font color=\"red\">Status: " + info + "</font><br>");
    }

    mStatusInfoTime = 80;
    mStatusLabel->setText(info);
}

void MainWindow::showMessageDialog(const QString &title, const QString &msg, bool isGood, bool richText)
{
    (void)richText;

    if (isGood) {
        QMessageBox::information(this, title, msg);
    } else {
        QMessageBox::warning(this, title, msg);
    }
}

void MainWindow::serialPortNotWritable(const QString &port)
{
    (void)port;

#ifdef Q_OS_LINUX
    QMessageBox::StandardButton reply;
    reply = QMessageBox::warning(this,
                                 tr("Connect Serial Port"),
                                 tr("The serial port is not writable. This can usually be fixed by "
                                    "adding your user to the dialout, uucp and/or lock groups. Would "
                                    "you like to do that?"),
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    if (reply == QMessageBox::Yes) {
        QString name = qgetenv("USER");
        if (name.isEmpty()) {
            name = qgetenv("USERNAME");
        }

        bool hasDialout = !runCmd("getent", QStringList() << "group" << "dialout").isEmpty();
        bool hasUucp = !runCmd("getent", QStringList() << "group" << "uucp").isEmpty();
        bool hasLock = !runCmd("getent", QStringList() << "group" << "lock").isEmpty();

        QString grps;
        if (hasDialout) {
            grps += "dialout";
        }

        if (hasUucp) {
            if (!grps.isEmpty()) {
                grps += ",";
            }
            grps += "uucp";
        }

        if (hasLock) {
            if (!grps.isEmpty()) {
                grps += ",";
            }
            grps += "lock";
        }

        QProcess process;
        process.setEnvironment(QProcess::systemEnvironment());
        process.start("pkexec", QStringList() << "usermod" << "-aG" << grps << name);
        waitProcess(process);
        if (process.exitCode() == 0) {
            showMessageDialog(tr("Command Result"),
                              tr("Result from command:\n\n"
                                 "%1\n"
                                 "You have to reboot for this "
                                 "change to take effect.").
                              arg(QString(process.readAllStandardOutput())),
                              true, false);
        } else {
            showMessageDialog(tr("Command Result"),
                              tr("Running command failed."),
                              false, false);
        }
        process.close();
    }
#endif
}

void MainWindow::bmsconfUpdated()
{
    mMcConfRead = true;
}

void MainWindow::bmsConfigCheckResult(QStringList paramsNotSet)
{
    if (!paramsNotSet.isEmpty()) {
        ParamDialog::showParams(tr("Parameters truncated"),
                                tr("The following parameters were truncated because they were set outside "
                                   "of their allowed limits."),
                                mDieBieMS->bmsConfig(),
                                paramsNotSet,
                                this);
    }
}

void MainWindow::on_actionReconnect_triggered()
{
    mDieBieMS->reconnectLastPort();
}

void MainWindow::on_actionDisconnect_triggered()
{
    mDieBieMS->disconnectPort();
}

void MainWindow::on_actionReboot_triggered()
{
    mDieBieMS->commands()->reboot();
}

void MainWindow::on_actionReadBMScconf_triggered()
{
    mDieBieMS->commands()->getBMSconf();
}

void MainWindow::on_actionReadBMScconfDefault_triggered()
{
    mDieBieMS->commands()->getBMSconfDefault();
}

void MainWindow::on_actionWriteBMScconf_triggered()
{
    mDieBieMS->commands()->setBMSconf();
}

void MainWindow::on_actionSaveBMSConfXml_triggered()
{
    QString path;
    path = QFileDialog::getSaveFileName(this,
                                        tr("Choose where to save the motor configuration XML file"),
                                        ".",
                                        tr("Xml files (*.xml)"));

    if (path.isNull()) {
        return;
    }

    if (!path.toLower().endsWith(".xml")) {
        path += ".xml";
    }

    bool res = mDieBieMS->bmsConfig()->saveXml(path, "bmsConfiguration");

    if (res) {
        showStatusInfo("Saved motor configuration", true);
    } else {
        showMessageDialog(tr("Save motor configuration"),
                          tr("Could not save motor configuration:<BR>"
                             "%1").arg(mDieBieMS->bmsConfig()->xmlStatus()),
                          false, false);
    }
}

void MainWindow::on_actionLoadBMSConfXml_triggered()
{
    QString path;
    path = QFileDialog::getOpenFileName(this,
                                        tr("Choose motor configuration file to load"),
                                        ".",
                                        tr("Xml files (*.xml)"));

    if (path.isNull()) {
        return;
    }

    bool res = mDieBieMS->bmsConfig()->loadXml(path, "bmsConfiguration");

    if (res) {
        showStatusInfo("Loaded motor configuration", true);
    } else {
        showMessageDialog(tr("Load motor configuration"),
                          tr("Could not load motor configuration:<BR>"
                             "%1").arg(mDieBieMS->bmsConfig()->xmlStatus()),
                          false, false);
    }
}

void MainWindow::on_actionExit_triggered()
{
    qApp->exit();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "DieBieMS Tool", Utility::aboutText());
}

void MainWindow::on_actionLibrariesUsed_triggered()
{
    QMessageBox::about(this, "Libraries Used",
                       tr("<b>Icons<br>"
                          "<a href=\"https://icons8.com/\">https://icons8.com/</a><br><br>"
                          "<b>Plotting<br>"
                          "<a href=\"http://qcustomplot.com/\">http://qcustomplot.com/</a>"));
}

void MainWindow::addPageItem(QString name, QString icon, QString groupIcon, bool bold, bool indented)
{
    QListWidgetItem *item = new QListWidgetItem();
    ui->pageList->addItem(item);
    PageListItem *li = new PageListItem(name, icon, groupIcon, this);
    li->setBold(bold);
    li->setIndented(indented);
    ui->pageList->setItemWidget(item, li);
}

void MainWindow::saveParamFileDialog(QString conf, bool wrapIfdef)
{
    ConfigParams *params = 0;

    if (conf.toLower() == "mcconf") {
        params = mDieBieMS->bmsConfig();
    } else {
        qWarning() << "Invalid conf" << conf;
        return;
    }

    QString path;
    path = QFileDialog::getSaveFileName(this,tr("Choose where to save the configuration header file"),".",tr("h files (*.h)"));

    if (path.isNull()) {
        return;
    }

    if (path.contains(" ")) {
        showMessageDialog(tr("Save header"),tr("Spaces are not allowed in the filename."),false, false);
        return;
    }

    if (!path.toLower().endsWith(".h")) {
        path += ".h";
    }

    bool res = params->saveCDefines(path, wrapIfdef);

    if (res) {
        showStatusInfo("Saved C header", true);
    } else {
        showMessageDialog(tr("Save header"),
                          tr("Could not save header"),
                          false, false);
    }
}

void MainWindow::showPage(const QString &name)
{
    for (int i = 0;i < ui->pageList->count();i++) {
        PageListItem *p = (PageListItem*)(ui->pageList->itemWidget(ui->pageList->item(i)));
        if (p->name() == name) {
            ui->pageList->setCurrentRow(i);
            break;
        }
    }
}

void MainWindow::reloadPages()
{
    // Remove pages (if any)
    ui->pageList->clear();
    while (ui->pageWidget->count() != 0) {
        QWidget* widget = ui->pageWidget->widget(0);
        ui->pageWidget->removeWidget(widget);
        widget->deleteLater();
    }

    mPageWelcome = new PageWelcome(this);
    mPageWelcome->setDieBieMS(mDieBieMS);
    ui->pageWidget->addWidget(mPageWelcome);
    addPageItem(tr("Welcome"), "://res/icons/Home-96.png", "", true);

    mPageConnection = new PageConnection(this);
    mPageConnection->setDieBieMS(mDieBieMS);
    ui->pageWidget->addWidget(mPageConnection);
    addPageItem(tr("Connection"), "://res/icons/Connected-96.png", "", true);

    mPageFirmware = new PageFirmware(this);
    mPageFirmware->setDieBieMS(mDieBieMS);
    ui->pageWidget->addWidget(mPageFirmware);
    addPageItem(tr("Firmware"), "://res/icons/Electronics-96.png", "", true);

    mPageMasterSettings = new PageMasterSettings(this);
    mPageMasterSettings->setDieBieMS(mDieBieMS);
    ui->pageWidget->addWidget(mPageMasterSettings);
    addPageItem(tr("Master Settings"), "://res/icons/Outgoing Data-96.png", "", true);

    mPageMasterGeneral = new PageMasterGeneral(this);
    mPageMasterGeneral->setDieBieMS(mDieBieMS);
    ui->pageWidget->addWidget(mPageMasterGeneral);
    addPageItem(tr("General"), "://res/icons/Horizontal Settings Mixer-96.png","", false, true);

    mPageMasterCell = new PageMasterCell(this);
    mPageMasterCell->setDieBieMS(mDieBieMS);
    ui->pageWidget->addWidget(mPageMasterCell);
    addPageItem(tr("Cell Management"), "://res/icons/batteries-96.png","", false, true);

    mPageMasterSwitch = new PageMasterSwitch(this);
    mPageMasterSwitch->setDieBieMS(mDieBieMS);
    ui->pageWidget->addWidget(mPageMasterSwitch);
    addPageItem(tr("Switch"), "://res/icons/Toggle Off-96_2.png","", false, true);

    mPageMasterDisplay = new PageMasterDisplay(this);
    mPageMasterDisplay->setDieBieMS(mDieBieMS);
    ui->pageWidget->addWidget(mPageMasterDisplay);
    addPageItem(tr("Display"), "://res/icons/Calculator-96.png","", false, true);

    mSlaveSettings = new PageSlaveSettings(this);
    mSlaveSettings->setDieBieMS(mDieBieMS);
    ui->pageWidget->addWidget(mSlaveSettings);
    addPageItem(tr("Slave Settings"), "://res/icons/Outgoing Data-96.png", "", true);

    mPageSlaveGeneral = new PageSlaveGeneral(this);
    mPageSlaveGeneral->setDieBieMS(mDieBieMS);
    ui->pageWidget->addWidget(mPageSlaveGeneral);
    addPageItem(tr("General"), "://res/icons/Horizontal Settings Mixer-96.png","", false, true);

    mPageSlaveSwitch = new PageSlaveSwitch(this);
    mPageSlaveSwitch->setDieBieMS(mDieBieMS);
    ui->pageWidget->addWidget(mPageSlaveSwitch);
    addPageItem(tr("Switch"), "://res/icons/Toggle Off-96_2.png","", false, true);

    mPageSlaveIO = new PageSlaveIO(this);
    mPageSlaveIO->setDieBieMS(mDieBieMS);
    ui->pageWidget->addWidget(mPageSlaveIO);
    addPageItem(tr("IO"), "://res/icons/Rs 232 Male-96.png","", false, true);

    mPageSlaveFANs = new PageSlaveFAN(this);
    mPageSlaveFANs->setDieBieMS(mDieBieMS);
    ui->pageWidget->addWidget(mPageSlaveFANs);
    addPageItem(tr("FANs"), "://res/icons/fan-96.png","", false, true);

    mPageDataAnalysis = new PageDataAnalysis(this);
    mPageDataAnalysis->setDieBieMS(mDieBieMS);
    ui->pageWidget->addWidget(mPageDataAnalysis);
    addPageItem(tr("Data Analysis"), "://res/icons/Line Chart-96.png", "", true);

    mPageRtData = new PageRtData(this);
    mPageRtData->setDieBieMS(mDieBieMS);
    ui->pageWidget->addWidget(mPageRtData);
    addPageItem(tr("Realtime Data"), "://res/icons/rt_off.png", "", false, true);

    mPageTerminal = new PageTerminal(this);
    mPageTerminal->setDieBieMS(mDieBieMS);
    ui->pageWidget->addWidget(mPageTerminal);
    addPageItem(tr("Terminal"), "://res/icons/Console-96.png", "", true);

    mPageDebugPrint = new PageDebugPrint(this);
    ui->pageWidget->addWidget(mPageDebugPrint);
    addPageItem(tr("Debug Console"), "://res/icons/Bug-96.png", "", true);

    mPageSettings = new PageSettings(this);
    ui->pageWidget->addWidget(mPageSettings);
    addPageItem(tr("Tool Settings"), "://res/icons/Settings-96.png", "", true);

    // Adjust sizes
    QFontMetrics fm(this->font());
    int width = fm.width("Welcome & Wizards++++++");
    int height = fm.height();

    for(int i = 0; i < ui->pageList->count(); i++) {
        QListWidgetItem *item = ui->pageList->item(i);
        item->setSizeHint(QSize(item->sizeHint().width(), height));
    }

    ui->pageList->setMinimumWidth(width);
    ui->pageList->setMaximumWidth(width);
    ui->pageLabel->setMaximumWidth(width);
    ui->pageLabel->setMaximumHeight((394 * width) / 1549);

    ui->pageList->setCurrentRow(0);
    ui->pageWidget->setCurrentIndex(0);
}

void MainWindow::checkUdev()
{
    // Check if udev rules for modemmanager are installed
#ifdef Q_OS_LINUX
    QFileInfo fi_mm("/lib/udev/rules.d/77-mm-usb-device-blacklist.rules");
    if (fi_mm.exists()) {
        QFileInfo fi_bms("/lib/udev/rules.d/45-vesc.rules");
        if (!fi_vesc.exists()) {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::information(this,
                                             tr("Modemmenager"),
                                             tr("It looks like modemmanager is installed on your system, and that "
                                                "there are no VESC udev rules installed. This will cause a delay "
                                                "from when you plug in the VESC until you can use it. Would you like "
                                                "to add a udev rule to prevent modemmanager from grabbing the VESC?"),
                                             QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

            if (reply == QMessageBox::Yes) {
                QFile f_bms(QDir::temp().absoluteFilePath(fi_vesc.fileName()));
                if (!f_vesc.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    showMessageDialog(tr("Create File Error"),
                                      f_vesc.errorString(),
                                      false, false);
                    return;
                }

                f_vesc.write("# Prevent modemmanager from grabbing the VESC\n"
                             "ATTRS{idVendor}==\"0483\", ATTRS{idProduct}==\"5740\", ENV{ID_MM_DEVICE_IGNORE}=\"1\"\n");
                f_vesc.close();

                QFileInfo fi_new(f_vesc);
                QProcess process;
                process.setEnvironment(QProcess::systemEnvironment());
                process.start("pkexec", QStringList() <<
                              "mv" <<
                              fi_new.absoluteFilePath() <<
                              fi_vesc.absolutePath());
                waitProcess(process);

                if (process.exitCode() == 0) {
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::information(this,
                                                     tr("Modemmenager"),
                                                     tr("The udev rule was created successfully. Would you like "
                                                        "to reload udev to apply the new rule?"),
                                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

                    if (reply == QMessageBox::Yes) {
                        QProcess process;
                        process.setEnvironment(QProcess::systemEnvironment());
                        process.start("pkexec", QStringList() <<
                                      "udevadm" <<
                                      "control" <<
                                      "--reload-rules");
                        waitProcess(process);

                        if (process.exitCode() == 0) {
                            showMessageDialog(tr("Command Result"),
                                              tr("Reloaded udev rules sucessfully."),
                                              true, false);
                        } else {
                            QString out = process.readAll();

                            if (out.isEmpty()) {
                                showMessageDialog(tr("Command Result"),
                                                  tr("Could not reload udev rules. A reboot is probably "
                                                     "required for this change to take effect."),
                                                  false, false);
                            }
                        }
                        process.close();
                    }
                } else {
                    showMessageDialog(tr("Command Result"),
                                      tr("Could not move rules file:\n\n"
                                         "%1").
                                      arg(QString(process.readAllStandardOutput())),
                                      false, false);
                }
                process.close();
            }
        }
    }
#endif
}

bool MainWindow::waitProcess(QProcess &process, bool block, int timeoutMs)
{
    bool wasEnables = isEnabled();
    bool killed = false;

    if (block) {
        setEnabled(false);
    }

    process.waitForStarted();

    QEventLoop loop;
    QTimer timeoutTimer;
    timeoutTimer.setSingleShot(true);
    timeoutTimer.start(timeoutMs);
    connect(&process, SIGNAL(finished(int)), &loop, SLOT(quit()));
    connect(&timeoutTimer, SIGNAL(timeout()), &loop, SLOT(quit()));
    loop.exec();

    if (process.state() == QProcess::Running) {
        process.kill();
        process.waitForFinished();
        killed = true;
    }

    setEnabled(wasEnables);

    return !killed;
}

QString MainWindow::runCmd(QString cmd, QStringList args)
{
    QProcess process;
    process.setEnvironment(QProcess::systemEnvironment());
    process.start(cmd, args);
    waitProcess(process);
    QString res = process.readAllStandardOutput();
    process.close();
    return res;
}

void MainWindow::on_pageList_currentRowChanged(int currentRow)
{
    if (currentRow >= 0) {
        ui->pageWidget->setCurrentIndex(currentRow);
    }
}

void MainWindow::on_actionParameterEditorBMScconf_triggered()
{
    ParameterEditor *p = new ParameterEditor(this);
    p->setAttribute(Qt::WA_DeleteOnClose);
    p->setParams(mDieBieMS->bmsConfig());
    p->show();
}

void MainWindow::on_actionParameterEditorInfo_triggered()
{
    ParameterEditor *p = new ParameterEditor(this);
    p->setAttribute(Qt::WA_DeleteOnClose);
    p->setParams(mDieBieMS->infoConfig());
    p->show();
}

void MainWindow::on_actionSaveBMSConfigurationHeader_triggered()
{
    saveParamFileDialog("mcconf", false);
}

void MainWindow::on_actionSaveBMSConfigurationHeaderWrap_triggered()
{
    saveParamFileDialog("mcconf", true);
}

void MainWindow::on_actionTerminalPrintFaults_triggered()
{
    mDieBieMS->commands()->sendTerminalCmd("faults");
    showPage("DieBieMS Terminal");
}

void MainWindow::on_actionTerminalShowHelp_triggered()
{
    mDieBieMS->commands()->sendTerminalCmd("help");
    showPage("DieBieMS Terminal");
}

void MainWindow::on_actionTerminalClear_triggered()
{
    mPageTerminal->clearTerminal();
    showPage("DieBieMS Terminal");
}

void MainWindow::on_actionTerminalPrintThreads_triggered()
{
    mDieBieMS->commands()->sendTerminalCmd("threads");
    showPage("DieBieMS Terminal");
}

void MainWindow::on_actionCanFwd_toggled(bool arg1)
{
    mDieBieMS->commands()->setSendCan(arg1);
}

void MainWindow::on_actionSafetyInformation_triggered()
{
    HelpDialog::showHelp(this, mDieBieMS->infoConfig(), "wizard_startup_usage");
}

void MainWindow::on_actionWarrantyStatement_triggered()
{
    HelpDialog::showHelp(this, mDieBieMS->infoConfig(), "wizard_startup_warranty");
}

void MainWindow::on_actionDieBieMSToolChangelog_triggered()
{
    HelpDialog::showHelp(this, "DieBieMS Tool Changelog", Utility::vescToolChangeLog());
}

void MainWindow::on_actionFirmwareChangelog_triggered()
{
    HelpDialog::showHelp(this, "Firmware Changelog", Utility::fwChangeLog());
}

void MainWindow::on_actionLicense_triggered()
{
    HelpDialog::showHelp(this, mDieBieMS->infoConfig(), "gpl_text");
}

void MainWindow::on_actionStoreBMScconf_triggered()
{
    mDieBieMS->commands()->storeBMSConfig();
}
