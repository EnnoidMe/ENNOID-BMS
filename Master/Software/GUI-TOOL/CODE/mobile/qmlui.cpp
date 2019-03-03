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

#include "qmlui.h"
#include "fwhelper.h"

#include <QQuickStyle>

QmlUi::QmlUi(QObject *parent) : QObject(parent)
{
    mEngine = new QQmlApplicationEngine(this);
}

bool QmlUi::startQmlUi()
{
    qmlRegisterSingletonType<BMSInterface>("Vedder.vesc.BMSInterface", 1, 0, "VescIf", BMSInterface_singletontype_provider);
    qmlRegisterSingletonType<Utility>("Vedder.vesc.utility", 1, 0, "Utility", utility_singletontype_provider);
    qmlRegisterType<BleUart>("Vedder.vesc.bleuart", 1, 0, "BleUart");
    qmlRegisterType<Commands>("Vedder.vesc.commands", 1, 0, "Commands");
    qmlRegisterType<ConfigParams>("Vedder.vesc.configparams", 1, 0, "ConfigParams");
    qmlRegisterType<FwHelper>("Vedder.vesc.fwhelper", 1, 0, "FwHelper");

    mEngine->load(QUrl(QLatin1String("qrc:/mobile/main.qml")));
    return !mEngine->rootObjects().isEmpty();
}

QObject *QmlUi::BMSInterface_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    (void)engine;
    (void)scriptEngine;

    BMSInterface *vesc = new BMSInterface();

    vesc->bmsConfig()->loadParamsXml("://res/parameters_bmsconfig.xml");
    //vesc->appConfig()->loadParamsXml("://res/parameters_appconf.xml");
    vesc->infoConfig()->loadParamsXml("://res/info.xml");

    return vesc;
}

QObject *QmlUi::utility_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    (void)engine;
    (void)scriptEngine;

    Utility *util = new Utility();

    return util;
}
