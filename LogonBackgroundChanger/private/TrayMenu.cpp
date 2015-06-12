#include "TrayMenu.hpp"
#include "SettingsWindow.hpp"
#include "EventProvider.hpp"

#include <QMenu>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QIcon>
#include <QSettings>
#include <QDebug>

TrayMenu::TrayMenu(QWidget *parent)
: QLabel(parent)
, m_ptrayIcon(nullptr)
, m_ptrayIconMenu(nullptr)
, m_psettingsWindow(nullptr)
, m_psettings(nullptr)
{
   setWindowTitle("logon background changer");

   m_psettingsWindow = new SettingsWindow(this);

   QAction *changeBackground = new QAction("&Change logon background", this);

   QMenu *changeIntervalSubMenu = new QMenu ("&Interval", this);
   QActionGroup *timeIntervalsActionGroup = new QActionGroup(this);
   QAction *never = new QAction("Never", this);
   QAction *oneMinute = new QAction("1 min", this);
   QAction *fiveMinutes = new QAction ("5 min", this);
   QAction *thirtyMinutes = new QAction ("30 min", this);
   QAction *oneHour = new QAction ("1 hour", this);
   QAction *onLocked = new QAction ("Every workstation lock", this);
   QAction *onUnlocked = new QAction ("Every workstation unlock", this);
   QAction *custom = new QAction ("Custom...", this);

   QAction *updateDirs = new QAction("Udpate directories", this);
   QAction *settings = new QAction("&Settings...", this);
   QAction *exit = new QAction("E&xit", this);

   m_psettings = new QSettings(this);

   never->setCheckable(true);
   oneMinute->setCheckable(true);
   fiveMinutes->setCheckable(true);
   thirtyMinutes->setCheckable(true);
   oneHour->setCheckable(true);
   onLocked->setCheckable(true);
   onUnlocked->setCheckable(true);
   custom->setCheckable(true);

   timeIntervalsActionGroup->addAction(never);
   timeIntervalsActionGroup->addAction(oneMinute);
   timeIntervalsActionGroup->addAction(fiveMinutes);
   timeIntervalsActionGroup->addAction(thirtyMinutes);
   timeIntervalsActionGroup->addAction(oneHour);
   timeIntervalsActionGroup->addAction(onLocked);
   timeIntervalsActionGroup->addAction(onUnlocked);
   timeIntervalsActionGroup->addAction(custom);

   changeIntervalSubMenu->addActions(timeIntervalsActionGroup->actions());

   connect(changeBackground, SIGNAL(triggered(bool)), SIGNAL(changeBackground()));

   connect(never, &QAction::triggered, [=](){ emit changeEvent(EventProvider::E_NONE, 0); });
   connect(oneMinute, &QAction::triggered, [=](){ emit changeEvent(EventProvider::E_TIME, 60*1000); });
   connect(fiveMinutes, &QAction::triggered, [=](){ emit changeEvent(EventProvider::E_TIME, 5*60*1000); });
   connect(thirtyMinutes, &QAction::triggered, [=](){ emit changeEvent(EventProvider::E_TIME, 30*60*1000); });
   connect(oneHour, &QAction::triggered, [=](){ emit changeEvent(EventProvider::E_TIME, 60*60*1000); });
   connect(onLocked, &QAction::triggered, [=](){ emit changeEvent(EventProvider::E_LOCK, 0); });
   connect(onUnlocked, &QAction::triggered, [=](){ emit changeEvent(EventProvider::E_UNLOCK, 0); });
   connect(custom, &QAction::triggered, [=](){ emit changeEvent(EventProvider::E_TIME, 24*60*60*1000); });
   connect(this, SIGNAL(changeEvent(EventProvider::eventType,int)), SLOT(saveSettings(EventProvider::eventType,int)));

   connect(updateDirs, SIGNAL(triggered(bool)), SIGNAL(settingsChanged()));
   connect(settings, SIGNAL(triggered()), this, SLOT(openSettings()));
   connect(exit, SIGNAL(triggered()), qApp, SLOT(quit()));
   connect(m_psettingsWindow, SIGNAL(settingsChanged()), SIGNAL(settingsChanged()));

   m_ptrayIconMenu = new QMenu(this);
   m_ptrayIconMenu->addAction(changeBackground);
   m_ptrayIconMenu->addMenu(changeIntervalSubMenu);
   m_ptrayIconMenu->addAction(updateDirs);
   m_ptrayIconMenu->addAction(settings);
   m_ptrayIconMenu->addAction(exit);
   m_ptrayIcon = new QSystemTrayIcon(this);

   m_ptrayIcon->setContextMenu(m_ptrayIconMenu);
   m_ptrayIcon->setToolTip("Logon background changer");
   m_ptrayIcon->setIcon(QIcon("://icon/icon.png"));
   m_ptrayIcon->show();

   if(m_psettings->contains("/event/type")){
       EventProvider::eventType event = static_cast<EventProvider::eventType>(m_psettings->value("/event/type").toInt());
       int time = m_psettings->value("/event/time").toInt();
       oneMinute->setChecked(event == EventProvider::E_TIME && time == 60*1000);
       fiveMinutes->setChecked(event == EventProvider::E_TIME && time == 5*60*1000);
       thirtyMinutes->setChecked(event == EventProvider::E_TIME && time == 30*60*1000);
       oneHour->setChecked(event == EventProvider::E_TIME && time == 60*60*1000);
       onLocked->setChecked(event == EventProvider::E_LOCK);
       onUnlocked->setChecked(event == EventProvider::E_UNLOCK);
       custom->setChecked(!oneMinute->isChecked()     &&
                          !fiveMinutes->isChecked()   &&
                          !thirtyMinutes->isChecked() &&
                          !oneHour->isChecked()       &&
                          !onLocked->isChecked()      &&
                          !onUnlocked->isChecked()    );

       emit changeEvent(event, time);
   } else {
       onLocked->setChecked(EventProvider::E_LOCK);
       emit changeEvent(EventProvider::E_LOCK, 0);
   }
}

TrayMenu::~TrayMenu()
{
}

void TrayMenu::openSettings()
{
   m_psettingsWindow->show();
}

void TrayMenu::showMessage(QString message)
{
    m_ptrayIcon->showMessage("Logon background changer", message, QSystemTrayIcon::Information, 3000);
}

void TrayMenu::saveSettings(EventProvider::eventType event, int time)
{
    m_psettings->setValue("/event/type", event);
    m_psettings->setValue("/event/time", time);
}

void TrayMenu::closeEvent(QCloseEvent *)
{
}
