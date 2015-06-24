#ifndef BACKGROUNDCHANGER_H
#define BACKGROUNDCHANGER_H

#include <QObject>
#include <QSize>
#include <QString>
#include <QImage>

class QSettings;
class QList<class T>;

class BackgroundChanger : public QObject
{
   Q_OBJECT
public:
   explicit BackgroundChanger(QObject *parent = 0);

   QString getWindowsDir();
signals:
   void message(QString message);
   void imageChanged(QImage image);

public slots:
   void updateImagesNames();
   void changeBackground();

private:
   QList<QString> m_images;
   QSettings *m_psettings;
   QSize m_screenSize;
   QString m_message;
};

#endif // BACKGROUNDCHANGER_H
