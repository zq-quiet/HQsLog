#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QString>
#include <thread>
#include "QsLog.h"
#include "QsLogDest.h"

void doWork()
{
    qint32 count =50000;
    while (count--)
    {
        QLOG_INFO()<<"  just test thread log";
        qInfo()<<" thread write log";
    }
}

using namespace QsLogging;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    // 1. init the logging mechanism
    Logger& logger = Logger::instance();
    logger.setLoggingLevel(QsLogging::TraceLevel);
    const QString sLogPath(QDir(a.applicationDirPath()).filePath("asynclog.txt"));


    DestinationPtr fileDestination(DestinationFactory::MakeAsyncFileDestination(
      sLogPath, EnableLogRotation, MaxSizeBytes(1024*1024), MaxOldLogCount(12)));
    logger.addDestination(fileDestination);
    qint32 i = 10000;

    std::thread work(doWork);

    while(i--)
    {
        QLOG_INFO()<<"  just test async log";
        qInfo()<<" main write log";
    }
    work.join();
    return a.exec();
}
