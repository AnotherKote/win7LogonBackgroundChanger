#include <QFile>
#include <QStringList>

#include "ConfigFileParser.hpp"

#include <QDebug>
ConfigFileParser::ConfigFileParser(QString pathToFile, QObject *parent)
: QObject(parent)
, file(nullptr)
{
    file = new QFile(this);  ///<@todo replace with QTextStream

    file->setFileName(pathToFile);
}

bool ConfigFileParser::parseFiles(QStringList &list)
{
    if(!file->exists()){
        QTextStream(stdout) << "Can't find config file \"" + file->fileName() + "\"\n";
        return false;
    }
    qDebug() << file->open(QIODevice::ReadOnly);
    qDebug() << file->canReadLine();

    while (!file->atEnd()) {
//        QString line.fromStdString(file->readLine().toStdString());
        QString line(file->readLine());
    }
    qDebug() << "finished";
}

