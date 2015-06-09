#include <QApplication>
#include <QTextStream>
#include <QFile>
#include <QStringList>

//#include "ConfigFileParser.hpp"
#include "WinEvent.hpp"
#include "TrayMenu.hpp"
#include "BackgroundChanger.hpp"

#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setOrganizationName("AnotherKote'sSoftware");
    QApplication::setApplicationName("LogonBackgroundChanger");
    QApplication::setQuitOnLastWindowClosed(false);

    WinEvent winEventRegistration;
    TrayMenu trayMenu;
    BackgroundChanger bgChanger; //QFileSystemWatcher

    QObject::connect(&trayMenu, SIGNAL(settingsChanged()), &bgChanger, SLOT(updateImagesNames()));
    QObject::connect(&trayMenu, SIGNAL(changeBackground()), &bgChanger, SLOT(changeBackground()));
    bgChanger.updateImagesNames();

    return a.exec();
}
