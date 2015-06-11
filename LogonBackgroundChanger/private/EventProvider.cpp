//#define _WIN32_WINNT 0x0601
#include <windows.h>
#include <wtsapi32.h>
#include <QLibrary>
#include <QTimer>

#include "EventProvider.hpp"

#include <QDebug>
EventProvider::EventProvider(QWidget *parent)
: QWidget(parent)
, m_timerTime(0)
, m_winStatusCode(UNREACHABLE_STATUS_CODE)
, m_ptimer(nullptr)
{
//   qDebug() << "WTSRegisterSessionNotification:" << WTSRegisterSessionNotification((HWND)this->winId(), (DWORD) NOTIFY_FOR_THIS_SESSION);
   QLibrary myLib("wtsapi32");

   myLib.load();
   myLib.loadHints();

   if (!myLib.isLoaded())
      exit(0);
   WTSRegisterSessionNotification = (MyPrototype2) myLib.resolve("WTSRegisterSessionNotification");
   if (!WTSRegisterSessionNotification)
      exit(0);
   else WTSRegisterSessionNotification((HWND)this->winId(), (DWORD) NOTIFY_FOR_THIS_SESSION);

   m_ptimer = new QTimer(this);
   connect(m_ptimer, &QTimer::timeout, [=](){
      emit timeToChange();
   });
}

EventProvider::~EventProvider()
{
}

void EventProvider::setEvent(EventProvider::eventType type, int time /*= 0*/)
{
//   m_timerTime = time;
   qDebug() << "type " << type << " time " << time;
   switch (type) {
   case E_NONE:
      m_winStatusCode = UNREACHABLE_STATUS_CODE;
      break;
   case E_TIME:
      m_winStatusCode = UNREACHABLE_STATUS_CODE;
      m_ptimer->setInterval(time);
      break;
   case E_UNLOCK:
      m_winStatusCode = WTS_SESSION_UNLOCK;
      break;
   case E_LOCK:
      m_winStatusCode = WTS_SESSION_LOCK;
      break;
   default:
      break;
   }

   if(type == E_TIME){
      m_ptimer->start();
   } else {
      m_ptimer->stop();
   }
}

bool EventProvider::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
   MSG *msg = reinterpret_cast<MSG*>(message);
   if( msg->message == WM_WTSSESSION_CHANGE ){
      if (msg->wParam == m_winStatusCode){
         emit timeToChange();
      }
   }
   return QWidget::nativeEvent(eventType, message, result);
}
