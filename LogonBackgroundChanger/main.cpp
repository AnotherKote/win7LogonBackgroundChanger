#include <QApplication>
#include <QTextStream>
#include <QFile>
#include <QStringList>

#include "ConfigFileParser.hpp"
#include "WinEvent.hpp"
#include "TrayMenu.hpp"

#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setOrganizationName("AnotherKote'sSoftware");
    QApplication::setApplicationName("LogonBackgroundChanger");

    QString pathToConfigFile = "lbgc.config";
    const QString usageArg = "-u";
    if(argc > 1){
        if(usageArg.compare(argv[1]) == 0){
            QTextStream(stdout) << "Usage: \n";
            QTextStream(stdout) << "\"lbgc.exe [PATH]\"\n";
            QTextStream(stdout) << "PATH - full path to config file. f.e.: \"D:\\myfiles\\config.file.txt\"\n";
            QTextStream(stdout) << "Config file example:\n";
            QTextStream(stdout) << "#comment \n";
            QTextStream(stdout) << "#-r after path used to recursively add all included folders\n";
            QTextStream(stdout) << "targetFolder=C:\\Windows\\System32\\oobe\\info\\backgrounds";
            QTextStream(stdout) << "targetFileName=defaultBackground.jpg";
            QTextStream(stdout) << "tilemode=this features is unavailable yet\n";
            QTextStream(stdout) << "path=D:\\path\\to\\folder\\with\\pictures\n";
            QTextStream(stdout) << "path=D:\\path\\to\\another\\folder\\with\\pictures -r\n";
            exit(0);
        } else {
            pathToConfigFile = argv[1];
        }
    } else {
        QTextStream(stdout) << "Executed with no args\n";
        QTextStream(stdout) << "For usage type \"lbgc.exe -u\"\n";
    }

    QTextStream(stdout) << "Using config file \"" + pathToConfigFile + "\"\n";

    ConfigFileParser parser(pathToConfigFile);
    QStringList list;
    parser.parseFiles(list);
    WinEvent aa;
    TrayMenu aaa;
//    aa.show();
    /// QStringList List of path's to all image files in path's of config file.
    /// QStringList list = parser.parseFile();
    /// if(list.empty)
    ///    "parsing failed";
    ///    "exiting";
    ///    exit(1);
    /// FileRandomChanger changer(QStringList) - change for random image from list, and converts it if needed.


    return a.exec();
}
