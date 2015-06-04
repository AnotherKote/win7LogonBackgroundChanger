#ifndef WINEVENT_H
#define WINEVENT_H

#include <QWidget>
#include <windows.h>

class WinEvent : public QWidget
{
   Q_OBJECT
public:
   explicit WinEvent(QWidget *parent = 0);
   ~WinEvent();

signals:

public slots:

   // QObject interface

protected:
   bool nativeEvent(const QByteArray &eventType, void *message, long *result);

private:
   typedef bool (*MyPrototype2)(HWND,DWORD);
    MyPrototype2 WTSRegisterSessionNotification;
};

#endif // WINEVENT_H
