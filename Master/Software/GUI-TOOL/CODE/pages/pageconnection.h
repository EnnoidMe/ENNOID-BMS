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

#ifndef PAGECONNECTION_H
#define PAGECONNECTION_H

#include <QWidget>
#include <QTimer>
#include "bmsinterface.h"

namespace Ui {
class PageConnection;
}

class PageConnection : public QWidget
{
    Q_OBJECT

public:
    explicit PageConnection(QWidget *parent = 0);
    ~PageConnection();

    BMSInterface *bms() const;
    void setDieBieMS(BMSInterface *dieBieMS);

private slots:
    void timerSlot();
    void bleScanDone(QVariantMap devs, bool done);

    void on_serialRefreshButton_clicked();
    void on_serialDisconnectButton_clicked();
    void on_serialConnectButton_clicked();
    void on_tcpDisconnectButton_clicked();
    void on_tcpConnectButton_clicked();
    void on_canFwdBox_valueChanged(int arg1);
    void on_helpButton_clicked();
    void on_canFwdButton_toggled(bool checked);
    void on_autoConnectButton_clicked();
    void on_bleScanButton_clicked();
    void on_bleDisconnectButton_clicked();
    void on_bleConnectButton_clicked();

private:
    Ui::PageConnection *ui;
    BMSInterface *mDieBieMS;
    QTimer *mTimer;

};

#endif // PAGECONNECTION_H
