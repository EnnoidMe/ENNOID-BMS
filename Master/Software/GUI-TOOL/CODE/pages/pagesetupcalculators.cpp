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

#include "pagesetupcalculators.h"
#include "ui_pagesetupcalculators.h"

PageSetupCalculators::PageSetupCalculators(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageSetupCalculators)
{
    ui->setupUi(this);
    layout()->setContentsMargins(0, 0, 0, 0);
    mDieBieMS = 0;

}

PageSetupCalculators::~PageSetupCalculators()
{
    delete ui;
}

void PageSetupCalculators::on_addSetupButton_clicked()
{

}

void PageSetupCalculators::on_removeSetupButton_clicked()
{

}

BMSInterface *PageSetupCalculators::bms() const
{
    return mDieBieMS;
}

void PageSetupCalculators::setDieBieMS(BMSInterface *dieBieMS)
{
    mDieBieMS = dieBieMS;
}
