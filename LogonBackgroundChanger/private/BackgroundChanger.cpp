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
#include <QBuffer>
#include <QFileSystemWatcher>

#include <windows.h>
#include <Shlobj.h>

#include "BackgroundChanger.hpp"
#include <QDebug>

BackgroundChanger::BackgroundChanger(QObject *parent)
: QObject(parent)
, m_defaultImagePath("\\backgrounds\\BackgroundDefault.jpg")
, m_myCoolImagePath("\\mycoolbackgrounds\\BackgroundDefault.jpg")
, m_psettings(nullptr)
, m_pimageWatcher(nullptr)
{
    m_pimageWatcher = new QFileSystemWatcher(this);

    connect(m_pimageWatcher, &QFileSystemWatcher::fileChanged, this,  &BackgroundChanger::imageChangedSlot);

    qDebug() << "add path " << m_pimageWatcher->addPath(getWindowsDir() + m_defaultImagePath); ///<@todo move to getWindowsDir()
    m_pimageWatcher->addPath(getWindowsDir() + m_myCoolImagePath);

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
    if(!m_images.isEmpty()){
        QImage image;
        int random = qrand()%m_images.count();
        qDebug() << QTime::currentTime().toString() << ":"  << image.load(m_images.at(random)) << " set image " << m_images.at(random) << " random " << random << " count " << m_images.count();
        int comressionLevel = 100;
        QBuffer buf;
        QImage scaledImage = image.scaled(m_screenSize);
        emit imageChanged(image);
        do{
            buf.close();
            buf.open(QIODevice::Truncate | QIODevice::ReadWrite);
            scaledImage.save(&buf, "JPG", comressionLevel--);
            //         emit message(QString::number(256000.0/buf.size()));
        } while (buf.size() >= 256000);
        qDebug() << buf.size();
        QFile file(getWindowsDir() + m_myCoolImagePath);
        qDebug() << " windiropen:" << file.open(QIODevice::Truncate | QIODevice::WriteOnly);
        file.write(buf.data()); ///<todo handle errors
        file.close();
        emit message("New image:\n" + QString(m_images.at(random)).replace("//", "\\"));
    } else {
        m_message = "No images was found in directories or no path set. Please, add some paths with images in settings.";
        emit message(m_message);
    }
}

void BackgroundChanger::imageChangedSlot(QString)
{
    //removing awful image
    QDir awfulImageDir(getWindowsDir() + "\\backgrounds"); // awfulImageDir: %WINDOWS%/System32/oobe/info/backgrounds
    awfulImageDir.removeRecursively();
    awfulImageDir.cdUp(); // awfulImageDir: %WINDOWS%/System32/oobe/info
    awfulImageDir.mkdir("backgrounds");

    //setting cool image
    QString coolImagePath(getWindowsDir() + m_myCoolImagePath);
    QFile coolBackground(coolImagePath);
    coolBackground.open(QIODevice::ReadOnly);
    coolBackground.copy(getWindowsDir() + m_defaultImagePath);

    //renew file system watcher path
    m_pimageWatcher->addPath(getWindowsDir() + m_defaultImagePath);
}

QString BackgroundChanger::getWindowsDir()
{
    WCHAR dir[MAX_PATH];
    SHGetFolderPath(0, CSIDL_WINDOWS, NULL, 0, dir);
//    QString path(QString::fromUtf16(reinterpret_cast<ushort*>(dir)) + "\\SysNative\\" + "oobe\\info\\backgrounds"); ///<@todo change sysnative to #ifdef x64 -> sysnative #else -> system32

    ///<3 luxoft
    QString path(QString::fromUtf16(reinterpret_cast<ushort*>(dir)) + "\\SysNative\\" + "oobe\\info");
    ///</3 luxoft

    QDir directory(path + "\\backgrounds");
    if(!directory.exists())
    {
        qDebug() << "!exists() " << directory.path();
        directory.mkpath(directory.path());
        QImage defaultImage(QString::fromUtf16(reinterpret_cast<ushort*>(dir)) + "\\SysNative\\" + "oobe\\background.bmp");
        defaultImage.save(directory.path() + "\\BackgroundDefault.jpg");

    }
    directory.setPath(path + "\\mycoolbackgrounds");
    if(!directory.exists())
    {
        qDebug() << "!exists() " << path;
        directory.mkpath(directory.path());
        QImage defaultImage(QString::fromUtf16(reinterpret_cast<ushort*>(dir)) + "\\SysNative\\" + "oobe\\background.bmp");
        defaultImage.save(directory.path() + "\\BackgroundDefault.jpg");
    }

    return QString::fromUtf16(reinterpret_cast<ushort*>(dir)) + "\\SysNative\\" + "oobe\\info";
}
