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

#include "utility.h"
#include <cmath>
#include <QProgressDialog>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QDebug>
#include <QNetworkReply>
#include <QMessageBox>

#ifdef Q_OS_ANDROID
#include <QtAndroid>
#include <QAndroidJniObject>
#endif

Utility::Utility(QObject *parent) : QObject(parent)
{

}

double Utility::map(double x, double in_min, double in_max, double out_min, double out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float Utility::throttle_curve(float val, float curve_acc, float curve_brake, int mode)
{
    float ret = 0.0;
    float val_a = fabsf(val);

    if (val < -1.0) {
        val = -1.0;
    }

    if (val > 1.0) {
        val = 1.0;
    }

    float curve;
    if (val >= 0.0) {
        curve = curve_acc;
    } else {
        curve = curve_brake;
    }

    // See
    // http://math.stackexchange.com/questions/297768/how-would-i-create-a-exponential-ramp-function-from-0-0-to-1-1-with-a-single-val
    if (mode == 0) { // Power
        if (curve >= 0.0) {
            ret = 1.0 - powf(1.0 - val_a, 1.0 + curve);
        } else {
            ret = powf(val_a, 1.0 - curve);
        }
    } else if (mode == 1) { // Exponential
        if (fabsf(curve) < 1e-10) {
            ret = val_a;
        } else {
            if (curve >= 0.0) {
                ret = 1.0 - ((expf(curve * (1.0 - val_a)) - 1.0) / (expf(curve) - 1.0));
            } else {
                ret = (expf(-curve * val_a) - 1.0) / (expf(-curve) - 1.0);
            }
        }
    } else if (mode == 2) { // Polynomial
        if (curve >= 0.0) {
            ret = 1.0 - ((1.0 - val_a) / (1.0 + curve * val_a));
        } else {
            ret = val_a / (1.0 - curve * (1.0 - val_a));
        }
    } else { // Linear
        ret = val_a;
    }

    if (val < 0.0) {
        ret = -ret;
    }

    return ret;
}

bool Utility::autoconnectBlockingWithProgress(BMSInterface *vesc, QWidget *parent)
{
    if (!vesc) {
        return false;
    }

    QProgressDialog dialog("Autoconnecting...", QString(), 0, 0, parent);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.show();

    bool res = vesc->autoconnect();

    if (!res) {
        vesc->emitMessageDialog(QObject::tr("Autoconnect"),QObject::tr("Could not autoconnect. Make sure that the USB cable is plugged in and that the DieBieMS is powered."),false);
    }

    return res;
}

void Utility::checkVersion(BMSInterface *dieBieMS)
{
    QString version = QString::number(DT_VERSION);
    QUrl url("https://vesc-project.com/vesctool-version.html");
    QNetworkAccessManager manager;
    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    QString res = QString::fromUtf8(reply->readAll());

    if (res.startsWith("vesctoolversion")) {
        res.remove(0, 15);
        res.remove(res.indexOf("vesctoolversion"), res.size());

        if (res.toDouble() > version.toDouble()) {
            if (dieBieMS) {
                //dieBieMS->emitStatusMessage("A new version of DieBieMS Tool is available", true);
            } else {
                //qDebug() << "A new version of DieBieMS Tool is available. Go to DeiBie.nl to download it and get all the latest features.";
            }
        }
    } else {
        qWarning() << res;
    }
}

QString Utility::fwChangeLog()
{
    QFile cl("://res/firmwares/CHANGELOG");
    if (cl.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QString::fromUtf8(cl.readAll());
    } else {
        return "";
    }
}

QString Utility::vescToolChangeLog()
{
    QFile cl("://res/CHANGELOG");
    if (cl.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QString::fromUtf8(cl.readAll());
    } else {
        return "";
    }
}

QString Utility::aboutText()
{
    return tr("<b>DieBieMS Tool %1</b><br>"
              "&copy; Danny Bokma 2018<br>"
              "<a href=\"mailto:bms@diebie.nl\">bms@diebie.nl</a><br>"
              "<a href=\"https://diebie.nl/\">https://diebie.nl/</a><br>"
              "Based on: ").
            arg(QString::number(DT_VERSION));
}

QString Utility::uuid2Str(QByteArray uuid, bool space)
{
    QString strUuid;
    for (int i = 0;i < uuid.size();i++) {
        QString str = QString::number(uuid.at(i), 16).
                rightJustified(2, '0').toUpper();
        strUuid.append(((i > 0 && space) ? " " : "") + str);
    }

    return strUuid;
}

bool Utility::requestFilePermission()
{
#ifdef Q_OS_ANDROID
    // Note: The following should work on Qt 5.10
    // https://codereview.qt-project.org/#/c/199162/
//    QtAndroid::PermissionResult r = QtAndroid::checkPermission("android.permission.WRITE_EXTERNAL_STORAGE");
//    if(r == QtAndroid::PermissionResult::Denied) {
//        QtAndroid::requestPermissionsSync( QStringList() << "android.permission.WRITE_EXTERNAL_STORAGE" );
//        r = QtAndroid::checkPermission("android.permission.WRITE_EXTERNAL_STORAGE");
//        if(r == QtAndroid::PermissionResult::Denied) {
//            return false;
//        }
//    }
    return true;
#else
    return true;
#endif
}
