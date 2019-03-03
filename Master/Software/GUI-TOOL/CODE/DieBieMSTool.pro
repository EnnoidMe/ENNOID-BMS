#-------------------------------------------------
#
# Project created by QtCreator 2016-08-12T21:55:19
#
#-------------------------------------------------

# Version
DT_VERSION = 0.21

DEFINES += DT_VERSION=$$DT_VERSION

# Serial port available
DEFINES += HAS_SERIALPORT

# Options
CONFIG += build_original

# Build mobile GUI
#CONFIG += build_mobile

#CONFIG += qtquickcompiler

QT       += core gui
QT       += widgets
QT       += printsupport
QT       += network
QT       += bluetooth
QT       += quick
QT       += quickcontrols2

contains(DEFINES, HAS_SERIALPORT) {
    QT       += serialport
}

android: QT += androidextras

android: TARGET = DieBieMSTool
!android: TARGET = DieBieMSToolV$$DT_VERSION

TEMPLATE = app

release_win {
    DESTDIR = build/win
    OBJECTS_DIR = build/win/obj
    MOC_DIR = build/win/obj
    RCC_DIR = build/win/obj
    UI_DIR = build/win/obj
}

release_lin {
    # http://micro.nicholaswilson.me.uk/post/31855915892/rules-of-static-linking-libstdc-libc-libgcc
    # http://insanecoding.blogspot.se/2012/07/creating-portable-linux-binaries.html
    QMAKE_LFLAGS += -static-libstdc++ -static-libgcc
    DESTDIR = build/lin
    OBJECTS_DIR = build/lin/obj
    MOC_DIR = build/lin/obj
    RCC_DIR = build/lin/obj
    UI_DIR = build/lin/obj
}

release_android {
    DESTDIR = build/android
    OBJECTS_DIR = build/android/obj
    MOC_DIR = build/android/obj
    RCC_DIR = build/android/obj
    UI_DIR = build/android/obj
}

build_mobile {
    DEFINES += USE_MOBILE
}

SOURCES += main.cpp\
        mainwindow.cpp \
    packet.cpp \
    vbytearray.cpp \
    commands.cpp \
    configparams.cpp \
    configparam.cpp \
    parametereditor.cpp \
    digitalfiltering.cpp \
    bleuart.cpp \
    utility.cpp \
    bmsinterface.cpp

HEADERS  += mainwindow.h \
    packet.h \
    vbytearray.h \
    commands.h \
    datatypes.h \
    configparams.h \
    configparam.h \
    parametereditor.h \
    digitalfiltering.h \
    bleuart.h \
    utility.h \
    bmsinterface.h

FORMS    += mainwindow.ui \
    parametereditor.ui

include(pages/pages.pri)
include(widgets/widgets.pri)
include(mobile/mobile.pri)

RESOURCES += res.qrc

build_original {
    DEFINES += VER_ORIGINAL
}

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
