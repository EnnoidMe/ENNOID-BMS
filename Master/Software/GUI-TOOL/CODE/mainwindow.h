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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QProcess>
#include <QSettings>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QListWidgetItem>
#include <cmath>
#include <QEventLoop>
#include <QDesktopServices>
#include <QLoggingCategory>

#include "bmsinterface.h"
#include "ui_mainwindow.h"
#include "parametereditor.h"
#include "utility.h"

#include "widgets/pagelistitem.h"
#include "widgets/helpdialog.h"
#include "widgets/paramdialog.h"
#include "pages/pagewelcome.h"
#include "pages/pageconnection.h"
#include "pages/pagedataanalysis.h"
#include "pages/pagertdata.h"
#include "pages/pageterminal.h"
#include "pages/pagefirmware.h"
#include "pages/pagedebugprint.h"
#include "pages/pagemastersettings.h"
#include "pages/pagemastergeneral.h"
#include "pages/pagemasterswitch.h"
#include "pages/pagemastercell.h"
#include "pages/pagemasterdisplay.h"
#include "pages/pageslavesettings.h"
#include "pages/pageslavegeneral.h"
#include "pages/pageslaveswitch.h"
#include "pages/pageslaveio.h"
#include "pages/pageslavefan.h"

#include "pages/pagesettings.h"




namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void timerSlot();
    void showStatusInfo(QString info, bool isGood);
    void showMessageDialog(const QString &title, const QString &msg, bool isGood, bool richText);
    void serialPortNotWritable(const QString &port);
    void bmsconfUpdated();
    void bmsConfigCheckResult(QStringList paramsNotSet);
    void on_actionReconnect_triggered();
    void on_actionDisconnect_triggered();
    void on_actionReboot_triggered();
    void on_actionReadBMScconf_triggered();
    void on_actionReadBMScconfDefault_triggered();
    void on_actionWriteBMScconf_triggered();
    void on_actionSaveBMSConfXml_triggered();
    void on_actionLoadBMSConfXml_triggered();
    void on_actionExit_triggered();
    void on_actionAbout_triggered();
    void on_actionLibrariesUsed_triggered();
    void on_pageList_currentRowChanged(int currentRow);
    void on_actionParameterEditorBMScconf_triggered();
    void on_actionParameterEditorInfo_triggered();
    void on_actionSaveBMSConfigurationHeader_triggered();
    void on_actionSaveBMSConfigurationHeaderWrap_triggered();
    void on_actionTerminalPrintFaults_triggered();
    void on_actionTerminalShowHelp_triggered();
    void on_actionTerminalClear_triggered();
    void on_actionTerminalPrintThreads_triggered();
    void on_actionCanFwd_toggled(bool arg1);
    void on_actionSafetyInformation_triggered();
    void on_actionWarrantyStatement_triggered();
    void on_actionDieBieMSToolChangelog_triggered();
    void on_actionFirmwareChangelog_triggered();
    void on_actionLicense_triggered();

    void on_actionStoreBMScconf_triggered();

private:
    Ui::MainWindow *ui;

    QSettings mSettings;
    QString mVersion;
    BMSInterface *mDieBieMS;
    QTimer *mTimer;
    QLabel *mStatusLabel;
    int mStatusInfoTime;
    bool mKeyLeft;
    bool mKeyRight;
    bool mMcConfRead;

    PageWelcome *mPageWelcome;
    PageConnection *mPageConnection;
    PageDataAnalysis *mPageDataAnalysis;
    PageRtData *mPageRtData;
    PageTerminal *mPageTerminal;
    PageFirmware *mPageFirmware;
    PageDebugPrint *mPageDebugPrint;
    PageMasterSettings *mPageMasterSettings;
    PageMasterGeneral *mPageMasterGeneral;
    PageMasterSwitch *mPageMasterSwitch;
    PageMasterCell *mPageMasterCell;
    PageMasterDisplay *mPageMasterDisplay;
    PageSlaveSettings *mSlaveSettings;
    PageSlaveGeneral *mPageSlaveGeneral;
    PageSlaveSwitch *mPageSlaveSwitch;
    PageSlaveIO *mPageSlaveIO;
    PageSlaveFAN *mPageSlaveFANs;

    PageSettings *mPageSettings;



    void addPageItem(QString name, QString icon = "", QString groupIcon = "", bool bold = false, bool indented = false);
    void saveParamFileDialog(QString conf, bool wrapIfdef);
    void showPage(const QString &name);
    void reloadPages();
    void checkUdev();
    bool waitProcess(QProcess &process, bool block = true, int timeoutMs = 300000);
    QString runCmd(QString cmd, QStringList args);

};

#endif // MAINWINDOW_H
