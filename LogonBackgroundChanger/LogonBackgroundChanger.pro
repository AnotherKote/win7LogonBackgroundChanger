#-------------------------------------------------
#
# Project created by QtCreator 2015-06-03T17:25:56
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LogonBackgroundChanger
CONFIG   += c++11

TEMPLATE = app

RC_FILE = icon.rc

SOURCES += main.cpp \
    private\TrayMenu.cpp \
    private\SettingsWindow.cpp \
    private\BackgroundChanger.cpp \
    private\EventProvider.cpp \
    private\RegisterTweaker.cpp \
    private/MenuImage.cpp

HEADERS += \
    TrayMenu.hpp \
    SettingsWindow.hpp \
    BackgroundChanger.hpp \
    EventProvider.hpp \
    RegisterTweaker.hpp \
    MenuImage.hpp

RESOURCES += qrc.qrc
