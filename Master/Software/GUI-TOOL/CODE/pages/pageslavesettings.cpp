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

#include "pageslavesettings.h"
#include "ui_pageslavesettings.h"

PageSlaveSettings::PageSlaveSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageSlaveSettings)
{
    ui->setupUi(this);
    layout()->setContentsMargins(0, 0, 0, 0);
    mDieBieMS = 0;
}

PageSlaveSettings::~PageSlaveSettings()
{
    delete ui;
}

BMSInterface *PageSlaveSettings::bms() const
{
    return mDieBieMS;
}

void PageSlaveSettings::setDieBieMS(BMSInterface *dieBieMS)
{
    mDieBieMS = dieBieMS;

    if (mDieBieMS) {
        ConfigParam *p = mDieBieMS->infoConfig()->getParam("slave_setting_description");
        if (p != 0) {
            ui->textEdit->setHtml(p->description);
        } else {
            ui->textEdit->setText("Slave Setting Description not found.");
        }
    }
}
