#ifndef EVENTPROVIDER_H
#define EVENTPROVIDER_H

#include <QWidget>
#include <windows.h> ///<@todo move to cpp

class EventProvider : public QWidget
{
   Q_OBJECT
public:
   explicit EventProvider(QWidget *parent = 0);
   ~EventProvider();

   enum eventType{
      E_NONE = 0,
      E_TIME,
      E_UNLOCK,
      E_LOCK
   };

signals:
   void timeToChange();

public slots:
   void setEvent(EventProvider::eventType type, int time);

protected:
   bool nativeEvent(const QByteArray &eventType, void *message, long *result);

private:
   static const long UNREACHABLE_STATUS_CODE = 6000;
   int m_timerTime;
   unsigned long m_winStatusCode;
   QTimer *m_ptimer;
   typedef bool (*MyPrototype2)(HWND,DWORD);
   MyPrototype2 WTSRegisterSessionNotification;
};

#endif // EVENTPROVIDER_H
