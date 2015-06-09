#ifndef BACKGROUNDCHANGER_H
#define BACKGROUNDCHANGER_H

#include <QObject>
#include <QSize>
#include <QString>

class QSettings;
class QList<class T>;

class BackgroundChanger : public QObject
{
   Q_OBJECT
public:
   explicit BackgroundChanger(QObject *parent = 0);

signals:

public slots:
   void updateImagesNames();
   void changeBackground();

private:
   QList<QString> m_images;
   QSettings *m_psettings;
   QSize m_screenSize;
   QString getWindowsDir();
};

#endif // BACKGROUNDCHANGER_H
