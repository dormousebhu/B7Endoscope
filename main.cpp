#include "mainwindow.h"
#include <QDebug>
#include <QApplication>
#include <QStandardPaths>
#include <QDate>
#include "MessageLogger.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Qly::LoggerController logger;
    logger.attach(new Qly::FileLogger("stderr", true));
    QString path = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    path = path + "/B7Endoscope_" + QDate::currentDate().toString("yyyyMMdd.txt");
    qDebug() << path;
    logger.attach(new Qly::FileLogger(path, true));
    logger.startLogging();

    MainWindow w;
    w.show();
    return a.exec();
}
