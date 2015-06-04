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

   m_psettingsWindow = new SettingsWindow();

   QAction *changeBackground = new QAction("&Change logon background", this);

   QMenu *changeIntervalSubMenu = new QMenu ("&Interval", this);
   QActionGroup *timeIntervalsActionGroup = new QActionGroup(this);
   QAction *oneMinute = new QAction("1 min", this);
   oneMinute->setCheckable(true);
   QAction *fiveMinutes = new QAction ("5 min", this);
   fiveMinutes->setCheckable(true);
   QAction *thirtyMinutes = new QAction ("30 min", this);
   thirtyMinutes->setCheckable(true);
   QAction *oneHour = new QAction ("1 hour", this);
   oneHour->setCheckable(true);
   QAction *onLocked = new QAction ("Every workstation lock", this);
   onLocked->setCheckable(true);
   QAction *onUnlocked = new QAction ("Every workstation unlock", this);
   onUnlocked->setCheckable(true);
   QAction *custom = new QAction ("Custom...", this);
   custom->setCheckable(true);
   timeIntervalsActionGroup->addAction(oneMinute);
   timeIntervalsActionGroup->addAction(fiveMinutes);
   timeIntervalsActionGroup->addAction(thirtyMinutes);
   timeIntervalsActionGroup->addAction(oneHour);
   timeIntervalsActionGroup->addAction(onLocked);
   timeIntervalsActionGroup->addAction(onUnlocked);
   timeIntervalsActionGroup->addAction(custom);

   changeIntervalSubMenu->addActions(timeIntervalsActionGroup->actions());

   QAction *settings = new QAction("&Settings...", this);
   connect(settings, SIGNAL(triggered()), this, SLOT(openSettings()));

   QAction *exit = new QAction("E&xit", this);
   connect(exit, SIGNAL(triggered()), qApp, SLOT(quit()));


   m_ptrayIconMenu = new QMenu(this);
   m_ptrayIconMenu->addAction(changeBackground);
   m_ptrayIconMenu->addMenu(changeIntervalSubMenu);
   m_ptrayIconMenu->addAction(settings);
   m_ptrayIconMenu->addAction(exit);
   m_ptrayIcon = new QSystemTrayIcon(this);

   m_ptrayIcon->setContextMenu(m_ptrayIconMenu);
   m_ptrayIcon->setToolTip("Logon background changer");
   m_ptrayIcon->show();
}

TrayMenu::~TrayMenu()
{
    delete m_psettingsWindow;
}

void TrayMenu::openSettings()
{
    m_psettingsWindow->show();
}



void TrayMenu::closeEvent(QCloseEvent *)
{
}
