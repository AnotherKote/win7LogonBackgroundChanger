#include <QList>
#include <QString>
#include <QDir>
#include <QSettings>
#include <QMessageBox>
#include <QTime>
#include <QApplication>
#include <QDesktopWidget>
#include <QImageWriter>
#include <QTime>
//#include <QImageReader>
#include <QBuffer>
#include <windows.h>
#include <Shlobj.h>

#include "BackgroundChanger.hpp"
#include <QDebug>

BackgroundChanger::BackgroundChanger(QObject *parent)
    : QObject(parent)
    , m_psettings(nullptr)
    , m_isRandom(false)
    , m_currentImageIndex(0)
{
   QTime midnight(0,0,0);
   qsrand(midnight.secsTo(QTime::currentTime()));

   m_psettings = new QSettings(this);
   m_screenSize.setWidth(QApplication::desktop()->screenGeometry().width());
   m_screenSize.setHeight(QApplication::desktop()->screenGeometry().height());
}

void BackgroundChanger::updateImageNames()
{
    m_images.clear();
    QList<QString> dirs;
    if(m_psettings){
        if(!m_psettings->value("/Dirs/" + QString::number(0)).toString().isEmpty()){
            for(int index = 0; !m_psettings->value("/Dirs/" + QString::number(index)).toString().isEmpty(); ++index){  ///<@todo move /Dirs/ to class with constants
                dirs.append(m_psettings->value("/Dirs/" + QString::number(index)).toString());
            }

            QStringList filters; //working with dirs setFilter to filter dirs form files
            filters << "*.png" << "*.jpg" << "*.jpeg";
            QDir currentDir;
            currentDir.setNameFilters(filters);
            float dirIndex = 0;
            float entryIndex = 1;
            for(auto dir: dirs){
                currentDir.setPath(dir);
                entryIndex = 1;
                for(auto path: currentDir.entryList()){
                    m_images.append(dir + '/' + path);
                    //               qDebug() << "progress:" << ((dirIndex + entryIndex/currentDir.entryList().count())/dirs.count());
                    ///<@todo emit progress(dir.index/dirs.count() * path.index/currentDir.entryList().count())
                    //               emit message(QString::number(((dirIndex + entryIndex/currentDir.entryList().count())/dirs.count())));
                    entryIndex++;
                }
                dirIndex++;
            }
            if(m_images.isEmpty()){
                m_message = "No images was found in directories";
                emit message(m_message);
            } else {
                emit message( QString::number(m_images.count()) +
                              + " image" + ((m_images.count() == 1)? " " : "s ") +
                              + "found in " + QString::number(dirs.count()) + " director" +
                              + ((dirs.count() == 1)? "y" : "ies"));
            }
        } else {
            m_message = "No path set. Please, add some paths with images in settings.";
            emit message(m_message);
        }
    }
    ///<@todo emit updateDone();
}

void BackgroundChanger::changeBackground()
{
    qsrand(static_cast<uint>(QDateTime::currentMSecsSinceEpoch()));
    if(!m_images.isEmpty()){
        if(m_isRandom)
        {
            m_currentImageIndex = qrand()%m_images.count();
        }
        else
        {
            m_currentImageIndex = (m_currentImageIndex + 1 < m_images.count()) ? m_currentImageIndex + 1 : 0;
        }
        QImage image;
        qDebug() << QTime::currentTime().toString() << ":"  << image.load(m_images.at(m_currentImageIndex)) << " set image " << m_images.at(m_currentImageIndex) << " random " << m_currentImageIndex << " count " << m_images.count();
        int comressionLevel = 100;
        QBuffer buf;
        QImage scaledImage = image.scaled(m_screenSize, Qt::KeepAspectRatio);
        emit imageChanged(image);
        do{
           buf.close();
           buf.open(QIODevice::Truncate | QIODevice::ReadWrite);
           scaledImage.save(&buf, "JPG", comressionLevel--);
           emit message(QString::number((256000.0/buf.size())*100) + "%");
        } while (buf.size() >= 256000);
        buf.close();
        qDebug() << buf.size();
        QFile file(getWindowsDir());
        file.open(QIODevice::Truncate | QIODevice::WriteOnly);
        file.write(buf.data()); ///<todo handle errors
        emit message("New image:\n" + QString(m_images.at(m_currentImageIndex)).replace("//", "\\"));
    } else {
        m_message = "No images was found in directories or no path set. Please, add some paths with images in settings.";
        emit message(m_message);
    }
}

QString BackgroundChanger::getWindowsDir()
{
    WCHAR dir[MAX_PATH];
    SHGetFolderPath(0, CSIDL_WINDOWS, NULL, 0, dir);
    return QString::fromUtf16(reinterpret_cast<ushort*>(dir)) + "\\SysNative\\" + "oobe\\info\\backgrounds\\BackgroundDefault.jpg";
}

void BackgroundChanger::setIsRandom(bool isRandom)
{
    m_isRandom = isRandom;
}
