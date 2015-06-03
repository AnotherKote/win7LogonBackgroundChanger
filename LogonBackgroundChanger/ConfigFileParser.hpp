#ifndef CONFIGFILEPARSER_H
#define CONFIGFILEPARSER_H

#include <QObject>

class QFile;
class QStringList;

class ConfigFileParser : public QObject
{
    Q_OBJECT
public:
    explicit ConfigFileParser(QString pathToFile, QObject *parent = 0);

    bool parseFiles(QStringList &list);
signals:

public slots:

private:
    QFile *file;
};

#endif // CONFIGFILEPARSER_H
