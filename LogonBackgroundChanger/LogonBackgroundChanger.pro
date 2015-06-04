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

SOURCES += main.cpp \
    ConfigFileParser.cpp \
    WinEvent.cpp \
    TrayMenu.cpp \
    SettingsWindow.cpp

HEADERS += ConfigFileParser.hpp \
    WinEvent.hpp \
    TrayMenu.hpp \
    SettingsWindow.hpp
