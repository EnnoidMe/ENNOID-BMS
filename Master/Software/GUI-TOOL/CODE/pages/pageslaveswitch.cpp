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

#include "pageslaveswitch.h"
#include "ui_pageslaveswitch.h"

PageSlaveSwitch::PageSlaveSwitch(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageSlaveSwitch)
{
    ui->setupUi(this);
    mDieBieMS = 0;
}

PageSlaveSwitch::~PageSlaveSwitch()
{
    delete ui;
}

BMSInterface *PageSlaveSwitch::bms() const {
    return mDieBieMS;
}

void PageSlaveSwitch::setDieBieMS(BMSInterface *dieBieMS) {
    mDieBieMS = dieBieMS;

    if (mDieBieMS) {
        ui->switchTab->addParamRow(mDieBieMS->bmsConfig(), "HCUseRelay");
        ui->switchTab->addParamRow(mDieBieMS->bmsConfig(), "HCUsePrecharge");
        ui->switchTab->addParamRow(mDieBieMS->bmsConfig(), "togglePowerModeDirectHCDelay");
        ui->switchTab->addParamRow(mDieBieMS->bmsConfig(), "timeoutHCPreCharge");
        ui->switchTab->addParamRow(mDieBieMS->bmsConfig(), "timeoutHCPreChargeRetryInterval");
        ui->switchTab->addParamRow(mDieBieMS->bmsConfig(), "timeoutHCRelayOverlap");
    }
}
