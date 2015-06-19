#ifndef REGISTERTWEAKER_H
#define REGISTERTWEAKER_H

#include <QObject>

class QSettings;

class RegisterTweaker: public QObject
{
   Q_OBJECT

public:
   RegisterTweaker(QObject *parent = 0);
   ~RegisterTweaker();

public slots:
   void tweakRegister();
   void checkRegister();
signals:
   void result(bool);
   void message(QString);
};

#endif // REGISTERTWEAKER_H
