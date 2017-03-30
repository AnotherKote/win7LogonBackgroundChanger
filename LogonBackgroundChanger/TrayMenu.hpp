#ifndef TRAYMENU_H
#define TRAYMENU_H

#include <QLabel>
#include <QImage>
#include <EventProvider.hpp>
//#include <QString>

class QSystemTrayIcon;
class QMenu;
class SettingsWindow;
class QIcon;
class QSettings;
class QAction;
class QWidgetAction;
class MenuImage;

class TrayMenu : public QLabel
{
   Q_OBJECT
private:
   QSystemTrayIcon *m_ptrayIcon;
   QMenu *m_ptrayIconMenu;
   SettingsWindow *m_psettingsWindow;
   QSettings *m_psettings;
   QAction *m_pchangeBackground;
   QAction *m_pRandom;
   QAction *m_pexitAction;
   QAction *m_ptweakRegisterAction;
   QWidgetAction *m_pcurrentImage;
   MenuImage *m_pimageLabel;

   QAction *m_never;
   QAction *m_oneMinute;
   QAction *m_fiveMinutes;
   QAction *m_thirtyMinutes;
   QAction *m_oneHour;
   QAction *m_onLocked;
   QAction *m_onUnlocked;
   QAction *m_onLogon;
   QAction *m_custom;

public:
   TrayMenu(QWidget *parent = 0);
   ~TrayMenu();

   void readSettings();
public slots:
   void openSettings();
   void showMessage(QString message);
   void saveSettings(EventProvider::eventType event, int time);
   void saveRandom(bool isRandom);
   void setActionsEnabled(bool enabled);
   void setCurrentPicture(QImage image);

protected:
   void closeEvent(QCloseEvent *);

signals:
   void settingsChanged();
   void changeBackground();
   void changeEvent(EventProvider::eventType, int);
   void tweakRegister();
   void randomChanged(bool isRandom);
};

#endif // TRAYMENU_H
