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

#ifndef DIGITALFILTERING_H
#define DIGITALFILTERING_H

#include <QVector>

class DigitalFiltering
{
public:
    DigitalFiltering();

    static void fft(int dir,int m,double *real,double *imag);
    static void dft(int dir, int len, double *real, double *imag);
    static void fftshift(double *data, int len);
    static void hamming(double *data, int len);
    static void zeroPad(double *data, double *result, int dataLen, int resultLen);
    static int whichPowerOfTwo(unsigned int number);

    static QVector<double> filterSignal(const QVector<double> &signal, const QVector<double> &filter, bool padAfter = false);
    static QVector<double> generateFirFilter(double f_break, int bits, bool useHamming);
    static QVector<double> fftWithShift(QVector<double> &signal, int resultBits, bool scaleByLen = false);
};

#endif // DIGITALFILTERING_H
