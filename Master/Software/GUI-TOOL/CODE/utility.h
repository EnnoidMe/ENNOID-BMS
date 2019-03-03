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

#ifndef UTILITY_H
#define UTILITY_H

#include <QObject>
#include "bmsinterface.h"

class Utility : public QObject
{
    Q_OBJECT

public:
    explicit Utility(QObject *parent = nullptr);

    static double map(double x, double in_min, double in_max, double out_min, double out_max);
    static float throttle_curve(float val, float curve_acc, float curve_brake, int mode);
    static bool autoconnectBlockingWithProgress(BMSInterface *vesc, QWidget *parent = 0);
    Q_INVOKABLE static void checkVersion(BMSInterface *vesc = 0);
    Q_INVOKABLE static QString fwChangeLog();
    Q_INVOKABLE static QString vescToolChangeLog();
    Q_INVOKABLE static QString aboutText();
    Q_INVOKABLE static QString uuid2Str(QByteArray uuid, bool space);
    Q_INVOKABLE static bool requestFilePermission();

signals:

public slots:
};

#endif // UTILITY_H
