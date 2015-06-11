#ifndef TRAYMENU_H
#define TRAYMENU_H

#include <QLabel>
#include <EventProvider.hpp>
//#include <QString>

class QSystemTrayIcon;
class QMenu;
class SettingsWindow;
class QIcon;

class TrayMenu : public QLabel
{
   Q_OBJECT
private:
   QSystemTrayIcon *m_ptrayIcon;
   QMenu *m_ptrayIconMenu;
   SettingsWindow *m_psettingsWindow;
public:
   TrayMenu(QWidget *parent = 0);
   ~TrayMenu();
public slots:
   void openSettings();
   void showMessage(QString message);
   // QWidget interface
protected:
   void closeEvent(QCloseEvent *);
signals:
   void settingsChanged();
   void changeBackground();
   void changeEvent(EventProvider::eventType, int);
};

#endif // TRAYMENU_H
