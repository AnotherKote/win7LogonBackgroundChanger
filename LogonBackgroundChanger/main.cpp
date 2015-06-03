#include <QCoreApplication>
#include <QTextStream>

#include <QDebug>
int main(int argc, char *argv[])
{
   QCoreApplication a(argc, argv);

   QString pathToConfigFile = "lbgc.config";
   const QString usageArg = "-u";
   if(argc > 1){
      if(usageArg.compare(argv[1]) == 0){
         QTextStream(stdout) << "Usage: \n";
         QTextStream(stdout) << "\"lbgc.exe [PATH]\"\n";
         QTextStream(stdout) << "PATH - full path to config file. f.e.: \"D:\\myfiles\\config.file.txt\"\n";
      } else {
         pathToConfigFile = argv[1];
      }
   } else {
      QTextStream(stdout) << "Executed with no args\n";
      QTextStream(stdout) << "For usage type \"lbgc.exe -u\"\n";
   }

   QTextStream(stdout) << "Using config file \"" + pathToConfigFile + "\"\n";

   return a.exec();
}
