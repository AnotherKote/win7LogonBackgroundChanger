#include "TrayMenu.hpp"
#include "SettingsWindow.hpp"
#include "EventProvider.hpp"
#include "MenuImage.hpp"

#include <QMenu>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QIcon>
#include <QSettings>
#include <QMessageBox>
#include <QDebug>
#include <QWidgetAction>
#include <QImage>

TrayMenu::TrayMenu(QWidget *parent)
: QLabel(parent)
, m_ptrayIcon(nullptr)
, m_ptrayIconMenu(nullptr)
, m_psettingsWindow(nullptr)
, m_psettings(nullptr)
, m_pchangeBackground(nullptr)
, m_pRandom(nullptr)
, m_pexitAction(nullptr)
, m_ptweakRegisterAction(nullptr)
, m_pcurrentImage(nullptr)
, m_pimageLabel(nullptr)
, m_never(nullptr)
, m_oneMinute(nullptr)
, m_fiveMinutes(nullptr)
, m_thirtyMinutes(nullptr)
, m_oneHour(nullptr)
, m_onLocked(nullptr)
, m_onUnlocked(nullptr)
, m_onLogon(nullptr)
, m_custom(nullptr)
{
   setWindowTitle("logon background changer");

   m_psettingsWindow = new SettingsWindow(this);

   m_pcurrentImage = new QWidgetAction(this);
   m_pimageLabel = new MenuImage(this);
   m_pchangeBackground = new QAction("&Change logon background", this);
   m_pRandom = new QAction("&Random", this);

   QMenu *changeIntervalSubMenu = new QMenu ("&Interval", this);
   QActionGroup *timeIntervalsActionGroup = new QActionGroup(this);
   m_never = new QAction("Never", this);
   m_oneMinute = new QAction("1 min", this);
   m_fiveMinutes = new QAction ("5 min", this);
   m_thirtyMinutes = new QAction ("30 min", this);
   m_oneHour = new QAction ("1 hour", this);
   m_onLocked = new QAction ("Every workstation lock", this);
   m_onUnlocked = new QAction ("Every workstation unlock", this);
   m_onLogon = new QAction ("Every logon", this);
   m_custom = new QAction ("Custom...", this);

   QAction *updateDirs = new QAction("Update directories", this);
   QAction *settings = new QAction("&Settings...", this);
   m_ptweakRegisterAction = new QAction("&Tweak register", this);
   m_pexitAction = new QAction("E&xit", this);

   m_psettings = new QSettings(this);

   m_pRandom->setCheckable(true);

   m_pcurrentImage->setDefaultWidget(m_pimageLabel);

   m_never->setCheckable(true);
   m_oneMinute->setCheckable(true);
   m_fiveMinutes->setCheckable(true);
   m_thirtyMinutes->setCheckable(true);
   m_oneHour->setCheckable(true);
   m_onLocked->setCheckable(true);
   m_onUnlocked->setCheckable(true);
   m_onLogon->setCheckable(true);
   m_custom->setCheckable(true);

   timeIntervalsActionGroup->addAction(m_never);
   timeIntervalsActionGroup->addAction(m_oneMinute);
   timeIntervalsActionGroup->addAction(m_fiveMinutes);
   timeIntervalsActionGroup->addAction(m_thirtyMinutes);
   timeIntervalsActionGroup->addAction(m_oneHour);
   timeIntervalsActionGroup->addAction(m_onLocked);
   timeIntervalsActionGroup->addAction(m_onUnlocked);
   timeIntervalsActionGroup->addAction(m_onLogon);
   timeIntervalsActionGroup->addAction(m_custom);

   changeIntervalSubMenu->addActions(timeIntervalsActionGroup->actions());

   connect(m_pimageLabel, SIGNAL(mouseReleased()), SIGNAL(changeBackground()));
   connect(m_pchangeBackground, SIGNAL(triggered(bool)), SIGNAL(changeBackground()));

   connect(m_pRandom, &QAction::toggled, [this](bool s) {emit randomChanged(s);});
   connect(this, SIGNAL(randomChanged(bool)), SLOT(saveRandom(bool)));

   connect(m_never, &QAction::triggered, [this](){ emit changeEvent(EventProvider::E_NONE, 0); });
   connect(m_oneMinute, &QAction::triggered, [this](){ emit changeEvent(EventProvider::E_TIME, 60*1000); });
   connect(m_fiveMinutes, &QAction::triggered, [this](){ emit changeEvent(EventProvider::E_TIME, 5*60*1000); });
   connect(m_thirtyMinutes, &QAction::triggered, [this](){ emit changeEvent(EventProvider::E_TIME, 30*60*1000); });
   connect(m_oneHour, &QAction::triggered, [this](){ emit changeEvent(EventProvider::E_TIME, 60*60*1000); });
   connect(m_onLocked, &QAction::triggered, [this](){ emit changeEvent(EventProvider::E_LOCK, 0); });
   connect(m_onUnlocked, &QAction::triggered, [this](){ emit changeEvent(EventProvider::E_UNLOCK, 0); });
   connect(m_onLogon, &QAction::triggered, [this](){ emit changeEvent(EventProvider::E_LOGON, 0); });
   connect(m_custom, &QAction::triggered, [this](){ emit changeEvent(EventProvider::E_TIME, 24*60*60*1000); });
   connect(this, SIGNAL(changeEvent(EventProvider::eventType,int)), SLOT(saveSettings(EventProvider::eventType,int)));

   connect(updateDirs, SIGNAL(triggered(bool)), SIGNAL(settingsChanged()));
   connect(settings, SIGNAL(triggered()), this, SLOT(openSettings()));
   connect(m_ptweakRegisterAction, SIGNAL(triggered(bool)), SIGNAL(tweakRegister()));
   connect(m_pexitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
   connect(m_psettingsWindow, SIGNAL(settingsChanged()), SIGNAL(settingsChanged()));

   m_ptrayIconMenu = new QMenu(this);
   m_ptrayIconMenu->addAction(m_pcurrentImage);
   m_ptrayIconMenu->addSeparator();
   m_ptrayIconMenu->addAction(m_pchangeBackground);
   m_ptrayIconMenu->addMenu(changeIntervalSubMenu);
   m_ptrayIconMenu->addAction(m_pRandom);
   m_ptrayIconMenu->addAction(updateDirs);
   m_ptrayIconMenu->addAction(settings);
   m_ptrayIconMenu->addAction(m_pexitAction);
   m_ptrayIcon = new QSystemTrayIcon(this);

   m_ptrayIcon->setContextMenu(m_ptrayIconMenu);
   m_ptrayIcon->setToolTip("Logon background changer");
   m_ptrayIcon->setIcon(QIcon("://icon/icon.png"));
   m_ptrayIcon->show();
}

TrayMenu::~TrayMenu()
{
}

void TrayMenu::readSettings()
{
    if(m_psettings->contains("/event/type")){
        EventProvider::eventType event = static_cast<EventProvider::eventType>(m_psettings->value("/event/type").toInt());
        int time = m_psettings->value("/event/time").toInt();

        m_never->setChecked(event == EventProvider::E_NONE);
        m_oneMinute->setChecked(event == EventProvider::E_TIME && time == 60*1000);
        m_fiveMinutes->setChecked(event == EventProvider::E_TIME && time == 5*60*1000);
        m_thirtyMinutes->setChecked(event == EventProvider::E_TIME && time == 30*60*1000);
        m_oneHour->setChecked(event == EventProvider::E_TIME && time == 60*60*1000);
        m_onLocked->setChecked(event == EventProvider::E_LOCK);
        m_onUnlocked->setChecked(event == EventProvider::E_UNLOCK);
        m_onLogon->setChecked(event == EventProvider::E_LOGON);
        m_custom->setChecked(event == EventProvider::E_CUSTOM);

        bool random = m_psettings->value("/event/random").toBool();
        m_pRandom->setChecked(random);

        emit changeEvent(event, time);
        emit randomChanged(random);
    } else {
        m_onLocked->setChecked(EventProvider::E_LOCK);
        m_pRandom->setChecked(true);

        emit changeEvent(EventProvider::E_LOCK, 0);
        emit randomChanged(true);
    }
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

void TrayMenu::saveRandom(bool isRandom)
{
    m_psettings->setValue("/event/random", isRandom);
}

void TrayMenu::closeEvent(QCloseEvent *)
{
}

void TrayMenu::setActionsEnabled(bool enabled)
{
   for (auto action: m_ptrayIconMenu->actions()){
      if (action  == m_pexitAction){
         action->setEnabled(true);
      }else{
         action->setEnabled(enabled);
      }
   }
   if (enabled){
      m_ptrayIconMenu->removeAction(m_ptweakRegisterAction);
   }else {
      m_ptrayIconMenu->addAction(m_ptweakRegisterAction);

      QMessageBox::StandardButton reply;
      reply = QMessageBox::question(this, "Info", "Register is not tweaked. Tweak it right now?\n\n"
                                                  "This tweak will only add registry DWORD value:\n"
                                                  "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Authentication\\LogonUI\\Background\\OEMBackground = 1\n"
                                                  "You can do it manually and rerun this program.",

                                    QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::Yes) {
         emit tweakRegister();
      }
   }
}

void TrayMenu::setCurrentPicture(QImage image)
{
   m_ptrayIconMenu->removeAction(m_pcurrentImage);
   m_pimageLabel->setPixmap(QPixmap::fromImage(image.scaledToWidth(800, Qt::SmoothTransformation)));
   m_ptrayIconMenu->insertAction(m_pchangeBackground ,m_pcurrentImage);
}
