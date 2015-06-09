#include "TrayMenu.hpp"
#include "SettingsWindow.hpp"

#include <QMenu>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QDebug>
TrayMenu::TrayMenu(QWidget *parent)
: QLabel(parent)
, m_ptrayIcon(nullptr)
, m_ptrayIconMenu(nullptr)
, m_psettingsWindow(nullptr)
{
   setWindowTitle("logon background changer");

   m_psettingsWindow = new SettingsWindow(this);

   QAction *changeBackground = new QAction("&Change logon background", this);

   QMenu *changeIntervalSubMenu = new QMenu ("&Interval", this);
   QActionGroup *timeIntervalsActionGroup = new QActionGroup(this);
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

   oneMinute->setCheckable(true);
   fiveMinutes->setCheckable(true);
   thirtyMinutes->setCheckable(true);
   oneHour->setCheckable(true);
   onLocked->setCheckable(true);
   onUnlocked->setCheckable(true);
   custom->setCheckable(true);

   timeIntervalsActionGroup->addAction(oneMinute);
   timeIntervalsActionGroup->addAction(fiveMinutes);
   timeIntervalsActionGroup->addAction(thirtyMinutes);
   timeIntervalsActionGroup->addAction(oneHour);
   timeIntervalsActionGroup->addAction(onLocked);
   timeIntervalsActionGroup->addAction(onUnlocked);
   timeIntervalsActionGroup->addAction(custom);

   changeIntervalSubMenu->addActions(timeIntervalsActionGroup->actions());

   connect(changeBackground, SIGNAL(triggered(bool)), SIGNAL(changeBackground()));
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
   m_ptrayIcon->show();
}

TrayMenu::~TrayMenu()
{
}

void TrayMenu::openSettings()
{
    m_psettingsWindow->show();
}

void TrayMenu::closeEvent(QCloseEvent *)
{
}
