//#define _WIN32_WINNT 0x0601
#include <windows.h>
#include <wtsapi32.h>
#include <QLibrary>

#include "WinEvent.hpp"

#include <QDebug>
WinEvent::WinEvent(QWidget *parent)
: QWidget(parent)
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

}

WinEvent::~WinEvent()
{
}

bool WinEvent::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
   MSG *msg = reinterpret_cast<MSG*>(message);
   if( msg->message == WM_WTSSESSION_CHANGE ){
      if (msg->wParam == WTS_SESSION_UNLOCK){
         qDebug () << "UNLOCKED!!";
      }
   }
   return QWidget::nativeEvent(eventType, message, result);
}
