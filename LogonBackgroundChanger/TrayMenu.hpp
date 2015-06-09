#ifndef TRAYMENU_H
#define TRAYMENU_H

#include <QLabel>

class QSystemTrayIcon;
class QMenu;
class SettingsWindow;

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
   // QWidget interface
protected:
   void closeEvent(QCloseEvent *);
signals:
   void settingsChanged();
   void changeBackground();
};

#endif // TRAYMENU_H
