#ifndef TRAYMENU_H
#define TRAYMENU_H

#include <QLabel>
#include <EventProvider.hpp>
//#include <QString>

class QSystemTrayIcon;
class QMenu;
class SettingsWindow;
class QIcon;
class QSettings;
class QAction;

class TrayMenu : public QLabel
{
   Q_OBJECT
private:
   QSystemTrayIcon *m_ptrayIcon;
   QMenu *m_ptrayIconMenu;
   SettingsWindow *m_psettingsWindow;
   QSettings *m_psettings;
   QAction *m_pexitAction;
   QAction *m_ptweakRegisterAction;

public:
   TrayMenu(QWidget *parent = 0);
   ~TrayMenu();

public slots:
   void openSettings();
   void showMessage(QString message);
   void saveSettings(EventProvider::eventType event, int time);
   void setActionsEnabled(bool enabled);

protected:
   void closeEvent(QCloseEvent *);

signals:
   void settingsChanged();
   void changeBackground();
   void changeEvent(EventProvider::eventType, int);
   void tweakRegister();
};

#endif // TRAYMENU_H
