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

#include "pagesettings.h"
#include "ui_pagesettings.h"

PageSettings::PageSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageSettings)
{
    ui->setupUi(this);
    layout()->setContentsMargins(0, 0, 0, 0);

    if (mSettings.contains("app_scale_factor")) {
        ui->uiScaleBox->setValue(mSettings.value("app_scale_factor").toDouble());
    }

    if (mSettings.contains("app_scale_auto")) {
        ui->uiAutoScaleBox->setChecked(mSettings.value("app_scale_auto").toBool());
    }

    ui->uiScaleBox->setEnabled(!ui->uiAutoScaleBox->isChecked());
}

PageSettings::~PageSettings()
{
    delete ui;
}

void PageSettings::on_uiScaleBox_valueChanged(double arg1)
{
    mSettings.setValue("app_scale_factor", arg1);
}

void PageSettings::on_uiAutoScaleBox_toggled(bool checked)
{
    mSettings.setValue("app_scale_auto", checked);
    ui->uiScaleBox->setEnabled(!checked);
}
