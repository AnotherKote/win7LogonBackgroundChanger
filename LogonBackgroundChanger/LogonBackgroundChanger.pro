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

#RC_FILE = my.rc

SOURCES += main.cpp \
    WinEvent.cpp \
    TrayMenu.cpp \
    SettingsWindow.cpp \
    BackgroundChanger.cpp

HEADERS += \
    WinEvent.hpp \
    TrayMenu.hpp \
    SettingsWindow.hpp \
    BackgroundChanger.hpp
