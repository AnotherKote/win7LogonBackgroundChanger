#include <QApplication>
#include <QTextStream>
#include <QFile>
#include <QStringList>
#include <QThread>
#include <QImage>

//#include "ConfigFileParser.hpp"
#include "EventProvider.hpp"
#include "TrayMenu.hpp"
#include "BackgroundChanger.hpp"
#include "RegisterTweaker.hpp"

#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setOrganizationName("AnotherKote'sSoftware");
    QApplication::setApplicationName("LogonBackgroundChanger");
    QApplication::setQuitOnLastWindowClosed(false);

//    qRegisterMetaType<EventProvider::eventType>("EventProvider::eventType");

    EventProvider eventProvider;
    TrayMenu trayMenu;

    QThread thread(&trayMenu);
    BackgroundChanger bgChanger; //QFileSystemWatcher

    RegisterTweaker rt;

    QObject::connect(&trayMenu, SIGNAL(settingsChanged()), &bgChanger, SLOT(updateImageNames()), Qt::QueuedConnection);
    QObject::connect(&thread, SIGNAL(started()), &bgChanger, SLOT(updateImageNames()), Qt::QueuedConnection);

    QObject::connect(&trayMenu, SIGNAL(changeBackground()), &bgChanger, SLOT(changeBackground()), Qt::QueuedConnection);
    QObject::connect(&eventProvider, SIGNAL(timeToChange()), &bgChanger, SLOT(changeBackground()), Qt::QueuedConnection);

    QObject::connect(&bgChanger, SIGNAL(message(QString)), &trayMenu, SLOT(showMessage(QString)), Qt::QueuedConnection);
    QObject::connect(&bgChanger, SIGNAL(imageChanged(QImage)), &trayMenu, SLOT(setCurrentPicture(QImage)));
    QObject::connect(&trayMenu, SIGNAL(changeEvent(EventProvider::eventType,int)), &eventProvider, SLOT(setEvent(EventProvider::eventType,int)));
    QObject::connect(&trayMenu, SIGNAL(randomChanged(bool)), &bgChanger, SLOT(setIsRandom(bool)));
    QObject::connect(&rt, SIGNAL(message(QString)), &trayMenu, SLOT(showMessage(QString)));
    QObject::connect(&rt, SIGNAL(result(bool)), &trayMenu, SLOT(setActionsEnabled(bool)));
    QObject::connect(&trayMenu, SIGNAL(tweakRegister()), &rt, SLOT(tweakRegister()));
    QObject::connect(&a, SIGNAL(aboutToQuit()), &thread, SLOT(terminate()));

    bgChanger.moveToThread(&thread);
    thread.start();

    rt.checkRegister();
    QImage defaultImage;
    defaultImage.load(bgChanger.getWindowsDir());
    trayMenu.setCurrentPicture(defaultImage);
    trayMenu.readSettings();

    return a.exec();
}
