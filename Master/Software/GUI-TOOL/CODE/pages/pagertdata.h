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

#ifndef PAGERTDATA_H
#define PAGERTDATA_H

#include <QWidget>
#include <QVector>
#include <QTimer>
#include "bmsinterface.h"
#include "ui_pagertdata.h"

namespace Ui {
class PageRtData;
}

class PageRtData : public QWidget
{
    Q_OBJECT

public:
    explicit PageRtData(QWidget *parent = 0);
    ~PageRtData();

    BMSInterface *bms() const;
    void setDieBieMS(BMSInterface *dieBieMS);

private slots:
    void timerSlot();
    void valuesReceived(BMS_VALUES values);
    void cellsReceived(int cellCount, QVector<double> cellVoltageArray);

    void on_zoomHButton_toggled(bool checked);
    void on_zoomVButton_toggled(bool checked);
    void on_rescaleButton_clicked();
    void on_tempShowBMSBox_toggled(bool checked);
    void on_tempShowBatteryBox_toggled(bool checked);

private:
    Ui::PageRtData *ui;
    BMSInterface *mDieBieMS;
    QTimer *mTimer;

    QVector<double> mPackVoltage;
    QVector<double> mLCLoadVoltage;
    QVector<double> mLCLoadCurrent;
    QVector<double> mHCLoadVoltage;
    QVector<double> mHCLoadCurrent;
    QVector<double> mAuxVoltage;
    QVector<double> mAuxCurrent;
    QVector<double> mCellVHigh;
    QVector<double> mCellVAverage;
    QVector<double> mCellVLow;
    QVector<double> mTempBMSHigh;
    QVector<double> mTempBMSAverage;
    QVector<double> mTempBattHigh;
    QVector<double> mTempBattAverage;
    QVector<double> mSeconds;

    double mSecondCounter;
    qint64 mLastUpdateTime;

    bool mUpdateValPlot;

    QCPBarsGroup *group;
    QCPBars *barsNormal;
    QCPBars *barsBalance;

    void appendDoubleAndTrunc(QVector<double> *vec, double num, int maxSize);
    void updateZoom();

};

#endif // PAGERTDATA_H
