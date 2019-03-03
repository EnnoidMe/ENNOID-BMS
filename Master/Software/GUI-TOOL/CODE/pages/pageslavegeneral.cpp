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

#include "pageslavegeneral.h"
#include "ui_pageslavegeneral.h"

PageSlaveGeneral::PageSlaveGeneral(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageSlaveGeneral)
{
    ui->setupUi(this);
    mDieBieMS = 0;
}

PageSlaveGeneral::~PageSlaveGeneral()
{
    delete ui;
}

BMSInterface *PageSlaveGeneral::bms() const {
    return mDieBieMS;
}

void PageSlaveGeneral::setDieBieMS(BMSInterface *dieBieMS) {
    mDieBieMS = dieBieMS;

    if (mDieBieMS) {
        ui->slaveSensorsTab->addRowSeparator(tr("Shunt"));
        ui->slaveSensorsTab->addParamRow(mDieBieMS->bmsConfig(), "shuntHCFactor");
        ui->slaveSensorsTab->addParamRow(mDieBieMS->bmsConfig(), "shuntHCOffset");
        ui->slaveSensorsTab->addRowSeparator(tr("NTC specifications"));
        ui->slaveSensorsTab->addParamRow(mDieBieMS->bmsConfig(), "NTCHiAmpPCBTopResistor");
        ui->slaveSensorsTab->addParamRow(mDieBieMS->bmsConfig(), "NTCHiAmpPCB25Deg");
        ui->slaveSensorsTab->addParamRow(mDieBieMS->bmsConfig(), "NTCHiAmpPCBBeta");
        ui->slaveSensorsTab->addParamRow(mDieBieMS->bmsConfig(), "NTCHiAmpExtTopResistor");
        ui->slaveSensorsTab->addParamRow(mDieBieMS->bmsConfig(), "NTCHiAmpExt25Deg");
        ui->slaveSensorsTab->addParamRow(mDieBieMS->bmsConfig(), "NTCHiAmpExtBeta");
    }
}
