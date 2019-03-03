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

#include "pageslaveio.h"
#include "ui_pageslaveio.h"

PageSlaveIO::PageSlaveIO(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageSlaveIO)
{
    ui->setupUi(this);
    mDieBieMS = 0;
}

PageSlaveIO::~PageSlaveIO()
{
    delete ui;
}

BMSInterface *PageSlaveIO::bms() const {
    return mDieBieMS;
}

void PageSlaveIO::setDieBieMS(BMSInterface *dieBieMS) {
    mDieBieMS = dieBieMS;

    if (mDieBieMS) {       
        ui->slaveIOTabAUX->addRowSeparator(tr("AUX0"));
        ui->slaveIOTabAUX->addParamRow(mDieBieMS->bmsConfig(), "AUX0OutputMode");
        ui->slaveIOTabAUX->addParamRow(mDieBieMS->bmsConfig(), "AUX0TemperatureSource");
        ui->slaveIOTabAUX->addParamRow(mDieBieMS->bmsConfig(), "AUX0CurrentSource");
        ui->slaveIOTabAUX->addParamRow(mDieBieMS->bmsConfig(), "AUX0CurrentThreshold");
        ui->slaveIOTabAUX->addParamRow(mDieBieMS->bmsConfig(), "AUX0TemperatureThreshold");
        ui->slaveIOTabAUX->addParamRow(mDieBieMS->bmsConfig(), "AUX0TurnOnDelay");
        ui->slaveIOTabAUX->addParamRow(mDieBieMS->bmsConfig(), "AUX0TurnOffDelay");

        ui->slaveIOTabAUX->addRowSeparator(tr("AUX1"));
        ui->slaveIOTabAUX->addParamRow(mDieBieMS->bmsConfig(), "AUX1OutputMode");
        ui->slaveIOTabAUX->addParamRow(mDieBieMS->bmsConfig(), "AUX1TemperatureSource");
        ui->slaveIOTabAUX->addParamRow(mDieBieMS->bmsConfig(), "AUX1CurrentSource");
        ui->slaveIOTabAUX->addParamRow(mDieBieMS->bmsConfig(), "AUX1CurrentThreshold");
        ui->slaveIOTabAUX->addParamRow(mDieBieMS->bmsConfig(), "AUX1TemperatureThreshold");
        ui->slaveIOTabAUX->addParamRow(mDieBieMS->bmsConfig(), "AUX1TurnOnDelay");
        ui->slaveIOTabAUX->addParamRow(mDieBieMS->bmsConfig(), "AUX1TurnOffDelay");
    }
}
