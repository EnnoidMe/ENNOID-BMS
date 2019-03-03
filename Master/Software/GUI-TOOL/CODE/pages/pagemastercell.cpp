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

#include "pagemastercell.h"
#include "ui_pagemastercell.h"

PageMasterCell::PageMasterCell(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageMasterCell)
{
    ui->setupUi(this);
    mDieBieMS = 0;
}

PageMasterCell::~PageMasterCell()
{
    delete ui;
}

BMSInterface *PageMasterCell::bms() const
{
    return mDieBieMS;
}

void PageMasterCell::setDieBieMS(BMSInterface *dieBieMS)
{
    mDieBieMS = dieBieMS;

    if (mDieBieMS) {
        ui->specificationsTab->addParamRow(mDieBieMS->bmsConfig(), "noOfCells");
        ui->specificationsTab->addParamRow(mDieBieMS->bmsConfig(), "batteryCapacity");
        ui->specificationsTab->addParamRow(mDieBieMS->bmsConfig(), "cellHardUnderVoltage");
        ui->specificationsTab->addParamRow(mDieBieMS->bmsConfig(), "cellHardOverVoltage");
        ui->specificationsTab->addParamRow(mDieBieMS->bmsConfig(), "cellLCSoftUnderVoltage");
        ui->specificationsTab->addParamRow(mDieBieMS->bmsConfig(), "cellHCSoftUnderVoltage");
        ui->specificationsTab->addParamRow(mDieBieMS->bmsConfig(), "cellSoftOverVoltage");
        ui->specificationsTab->addParamRow(mDieBieMS->bmsConfig(), "maxUnderAndOverVoltageErrorCount");
        ui->specificationsTab->addParamRow(mDieBieMS->bmsConfig(), "hysteresisDischarge");
        ui->specificationsTab->addParamRow(mDieBieMS->bmsConfig(), "hysteresisCharge");

        ui->balancingTab->addParamRow(mDieBieMS->bmsConfig(), "cellBalanceStart");
        ui->balancingTab->addParamRow(mDieBieMS->bmsConfig(), "cellBalanceDifferenceThreshold");
        ui->balancingTab->addParamRow(mDieBieMS->bmsConfig(), "cellBalanceUpdateInterval");
        ui->balancingTab->addParamRow(mDieBieMS->bmsConfig(), "maxSimultaneousDischargingCells");

        ui->throttlingTab->addRowSeparator(tr("Discharge"));
        ui->throttlingTab->addParamRow(mDieBieMS->bmsConfig(), "cellThrottleLowerStart");
        ui->throttlingTab->addParamRow(mDieBieMS->bmsConfig(), "cellThrottleLowerMargin");
        ui->throttlingTab->addParamRow(mDieBieMS->bmsConfig(), "throttleDisChargeIncreaseRate");
        ui->throttlingTab->addRowSeparator(tr("Charge"));
        ui->throttlingTab->addParamRow(mDieBieMS->bmsConfig(), "cellThrottleUpperStart");
        ui->throttlingTab->addParamRow(mDieBieMS->bmsConfig(), "cellThrottleUpperMargin");
        ui->throttlingTab->addParamRow(mDieBieMS->bmsConfig(), "throttleChargeIncreaseRate");

        ui->socTab->addParamRow(mDieBieMS->bmsConfig(), "soCMethod");
        ui->socTab->addParamRow(mDieBieMS->bmsConfig(), "timeoutChargeCompleted");
        ui->socTab->addParamRow(mDieBieMS->bmsConfig(), "timeoutChargingCompletedMinimalMismatch");
        ui->socTab->addParamRow(mDieBieMS->bmsConfig(), "maxMismatchThreshold");
        ui->socTab->addParamRow(mDieBieMS->bmsConfig(), "stateOfChargeStoreInterval");
    }
}





