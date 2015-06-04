#ifndef TRAYMENU_H
#define TRAYMENU_H

#include <QLabel>

class QSystemTrayIcon;
class QMenu;

class TrayMenu : public QLabel
{
   Q_OBJECT
private:
   QSystemTrayIcon *m_ptrayIcon;
   QMenu *m_ptrayIconMenu;

public:
   TrayMenu(QWidget *parent = 0);

   // QWidget interface
protected:
   void closeEvent(QCloseEvent *);
};

#endif // TRAYMENU_H
