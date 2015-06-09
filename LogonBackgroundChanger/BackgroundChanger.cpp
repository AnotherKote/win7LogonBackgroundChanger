#include <QList>
#include <QString>
#include <QDir>
#include <QSettings>
#include <QMessageBox>
#include <QTime>
#include <QApplication>
#include <QDesktopWidget>
#include <QImageWriter>
#include <windows.h>
#include <Shlobj.h>
#include <QTime>

#include "BackgroundChanger.hpp"
#include <QDebug>

BackgroundChanger::BackgroundChanger(QObject *parent)
   : QObject(parent)
   , m_psettings(nullptr)
{
   QTime midnight(0,0,0);
   qsrand(midnight.secsTo(QTime::currentTime()));
   m_psettings = new QSettings(this);
   m_screenSize.setWidth(QApplication::desktop()->screenGeometry().width());
   m_screenSize.setHeight(QApplication::desktop()->screenGeometry().height());
}

void BackgroundChanger::updateImagesNames()
{
   m_images.clear();
   QList<QString> dirs;
   if(m_psettings){
      if(!m_psettings->value("/Dirs/" + QString::number(0)).toString().isEmpty()){
         for(int index = 0; !m_psettings->value("/Dirs/" + QString::number(index)).toString().isEmpty(); ++index){  ///<@todo move /Dirs/ to class with constants
            dirs.append(m_psettings->value("/Dirs/" + QString::number(index)).toString());
         }

         QStringList filters; //working with dirs setFilter to filter dirs form files
         filters << "*.png" << "*.gif" << "*.jpg" << "*.jpeg";
         QDir currentDir;
         currentDir.setNameFilters(filters);
         for(auto dir: dirs){
            currentDir.setPath(dir);
            for(auto path: currentDir.entryList()){
               m_images.append(dir + '/' + path);
            }
         }
         if(m_images.isEmpty()){
            QMessageBox::information(nullptr, "Info", "No images was found in directories");
         }
      } else {
         QMessageBox::information(nullptr, "Info", "No path set. Please, add some paths with images in settings.");
      }
   }

}

void BackgroundChanger::changeBackground()
{
   if(!m_images.isEmpty()){
      QImage image;
      int random = qrand()%m_images.count();
      qDebug() << image.load(m_images.at(random)) << " set image " << m_images.at(random) << " random " << random << " count " << m_images.count();
      int comressionLevel = 100;
      QFile imageFile(getWindowsDir());
      do{
         image.scaled(m_screenSize).save(getWindowsDir(), "JPG", comressionLevel--);
         qDebug() << imageFile.size();
      } while (imageFile.size() > 250000);
   } else {
      QMessageBox::information(nullptr, "Info", "No images was found in directories or no path set. Please, add some paths with images in settings.");
   }
}

QString BackgroundChanger::getWindowsDir()
{
   WCHAR dir[MAX_PATH];
   SHGetFolderPath(0, CSIDL_WINDOWS, NULL, 0, dir);
   return QString::fromUtf16(reinterpret_cast<ushort*>(dir)) + "\\SysNative\\" + "oobe\\info\\backgrounds\\BackgroundDefault.jpg";
}
