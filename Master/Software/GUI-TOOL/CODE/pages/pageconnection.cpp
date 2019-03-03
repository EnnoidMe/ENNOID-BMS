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

#include "pageconnection.h"
#include "ui_pageconnection.h"
#include "widgets/helpdialog.h"
#include "utility.h"
#include <QMessageBox>
#include <QSettings>

PageConnection::PageConnection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageConnection)
{
    ui->setupUi(this);
    layout()->setContentsMargins(0, 0, 0, 0);

    QString lastTcpServer =
        QSettings().value("tcp_server", ui->tcpServerEdit->text()).toString();
    ui->tcpServerEdit->setText(lastTcpServer);

    int lastTcpPort =
        QSettings().value("tcp_port", ui->tcpPortBox->value()).toInt();
    ui->tcpPortBox->setValue(lastTcpPort);

    mDieBieMS = 0;
    mTimer = new QTimer(this);

    connect(mTimer, SIGNAL(timeout()),
            this, SLOT(timerSlot()));

    mTimer->start(20);
}

PageConnection::~PageConnection()
{
    delete ui;
}

BMSInterface *PageConnection::bms() const
{
    return mDieBieMS;
}

void PageConnection::setDieBieMS(BMSInterface *dieBieMS)
{
    mDieBieMS = dieBieMS;

    connect(mDieBieMS->bleDevice(), SIGNAL(scanDone(QVariantMap,bool)),this, SLOT(bleScanDone(QVariantMap,bool)));

    QString lastBleAddr = QSettings().value("ble_addr").toString();
    if (lastBleAddr != "") {
        QString setName = mDieBieMS->getBleName(lastBleAddr);

        QString name;
        if (!setName.isEmpty()) {
            name += setName;
            name += " [";
            name += lastBleAddr;
            name += "]";
        } else {
            name = lastBleAddr;
        }
        ui->bleDevBox->insertItem(0, name, lastBleAddr);
    }

    on_serialRefreshButton_clicked();
}

void PageConnection::timerSlot()
{
    if (mDieBieMS) {
        QString str = mDieBieMS->getConnectedPortName();
        if (str != ui->statusLabel->text()) {
            ui->statusLabel->setText(mDieBieMS->getConnectedPortName());
        }

        // CAN fwd
        if (ui->canFwdButton->isChecked() != mDieBieMS->commands()->getSendCan()) {
            ui->canFwdButton->setChecked(mDieBieMS->commands()->getSendCan());
        }

        if (ui->canFwdBox->value() != mDieBieMS->commands()->getCanSendId()) {
            ui->canFwdBox->setValue(mDieBieMS->commands()->getCanSendId());;
        }
    }
}

void PageConnection::bleScanDone(QVariantMap devs, bool done)
{
    if (done) {
        ui->bleScanButton->setEnabled(true);
    }

    ui->bleDevBox->clear();
    for (auto d: devs.keys()) {
        QString devName = devs.value(d).toString();
        QString addr = d;
        QString setName = mDieBieMS->getBleName(addr);

        if (!setName.isEmpty()) {
            QString name;
            name += setName;
            name += " [";
            name += addr;
            name += "]";
            ui->bleDevBox->insertItem(0, name, addr);
        } else if (devName.contains("VESC") || devName.contains("Metr Pro")) {
            QString name;
            name += devName;
            name += " [";
            name += addr;
            name += "]";
            ui->bleDevBox->insertItem(0, name, addr);
        } else {
            QString name;
            name += devName;
            name += " [";
            name += addr;
            name += "]";
            ui->bleDevBox->addItem(name, addr);
        }
    }
    ui->bleDevBox->setCurrentIndex(0);
}

void PageConnection::on_serialRefreshButton_clicked()
{
    if (mDieBieMS) {
        ui->serialPortBox->clear();
        QList<VSerialInfo_t> ports = mDieBieMS->listSerialPorts();
        foreach(const VSerialInfo_t &port, ports) {
            ui->serialPortBox->addItem(port.name, port.systemPath);
        }
        ui->serialPortBox->setCurrentIndex(0);
    }
}

void PageConnection::on_serialDisconnectButton_clicked()
{
    if (mDieBieMS) {
        mDieBieMS->disconnectPort();
    }
}

void PageConnection::on_serialConnectButton_clicked()
{
    if (mDieBieMS) {
        mDieBieMS->connectSerial(ui->serialPortBox->currentData().toString(),
                             ui->serialBaudBox->value());
    }
}

void PageConnection::on_tcpDisconnectButton_clicked()
{
    if (mDieBieMS) {
        mDieBieMS->disconnectPort();
    }
}

void PageConnection::on_tcpConnectButton_clicked()
{
    if (mDieBieMS) {
        QString tcpServer = ui->tcpServerEdit->text();
        int tcpPort = ui->tcpPortBox->value();
        mDieBieMS->connectTcp(tcpServer, tcpPort);
        QSettings().setValue("tcp_server", tcpServer);
        QSettings().setValue("tcp_port", tcpPort);
    }
}

void PageConnection::on_canFwdBox_valueChanged(int arg1)
{
    if (mDieBieMS) {
        mDieBieMS->commands()->setCanSendId(arg1);
    }
}

void PageConnection::on_helpButton_clicked()
{
    if (mDieBieMS) {
        HelpDialog::showHelp(this, mDieBieMS->infoConfig(), "help_can_forward");
    }
}

void PageConnection::on_canFwdButton_toggled(bool checked)
{
    if (mDieBieMS) {
        mDieBieMS->commands()->setSendCan(checked);
    }
}

void PageConnection::on_autoConnectButton_clicked()
{
    Utility::autoconnectBlockingWithProgress(mDieBieMS, this);
}

void PageConnection::on_bleScanButton_clicked()
{
    if (mDieBieMS) {
        mDieBieMS->bleDevice()->startScan();
        ui->bleScanButton->setEnabled(false);
    }
}

void PageConnection::on_bleDisconnectButton_clicked()
{
    if (mDieBieMS) {
        mDieBieMS->disconnectPort();
    }
}

void PageConnection::on_bleConnectButton_clicked()
{
    if (mDieBieMS) {
        if (ui->bleDevBox->count() > 0) {
            QString bleAddr = ui->bleDevBox->currentData().toString();
            mDieBieMS->connectBle(bleAddr);
            QSettings().setValue("ble_addr", bleAddr);
        }
    }
}
