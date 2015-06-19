#include <QSettings>
#include <windows.h>

#include "RegisterTweaker.hpp"
#include <QDebug>
RegisterTweaker::RegisterTweaker(QObject *parent)
: QObject(parent)
{
}

RegisterTweaker::~RegisterTweaker()
{

}

void RegisterTweaker::tweakRegister()
{
   HKEY key;
   DWORD data = 1;
   long retValue = 0;
   retValue = RegCreateKeyEx( HKEY_LOCAL_MACHINE,
                              TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Authentication\\LogonUI\\Background"), 0,
                              NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_WOW64_64KEY, NULL, &key, NULL);
   if( retValue == ERROR_SUCCESS )
   {
      retValue = RegSetValueEx(key, TEXT("OEMBackground"), 0, REG_DWORD, (BYTE*)&data, sizeof(data));
      if( retValue == ERROR_SUCCESS)
      {
         emit result(true);
         emit message("Register is successfully tweaked");
      } else {
         emit message("Error #" + QString::number(retValue) + " while setting value. Error code from Winerror.h");
      }
   } else {
      emit message("Error #" + QString::number(retValue) + " while creating key. Error code from Winerror.h");
   }
}

void RegisterTweaker::checkRegister()
{
   HKEY keyHandle;
   char rgValue [1024];
   DWORD size1;
   DWORD Type = REG_DWORD;
   long retValue = 0;
   retValue = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
                            TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Authentication\\LogonUI\\Background"), 0,
                            KEY_QUERY_VALUE | KEY_WOW64_64KEY, &keyHandle);
   if( retValue == ERROR_SUCCESS)
   {
      RegQueryValueEx( keyHandle, TEXT("OEMBackground"), NULL, &Type, (LPBYTE)rgValue, &size1 );
      retValue = RegQueryValueEx( keyHandle, TEXT("OEMBackground"), NULL, &Type, (LPBYTE)rgValue, &size1 );
      DWORD *pint = nullptr;
      pint = (DWORD*)&rgValue;
      if( retValue == ERROR_SUCCESS && (*pint) == 1){
         emit result(true);
      }else {
         emit result (false);
         emit message("Error #" + QString::number(retValue) + " while checking value. Error code from Winerror.h");
      }
   } else {
      emit result(false);
      emit message("Error #" + QString::number(retValue) + " while opening key. Error code from Winerror.h");
   }

   RegCloseKey(keyHandle);
}

