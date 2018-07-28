#include "clockthread.h"

void ClockThread::run()
{
    lastDate = QDate::fromString("170601", "yyMMdd").addYears(100);
    emit sendTime(lastDate.toString("yyyy年MM月dd日"));
    QTimer timer;
    connect(&timer, SIGNAL(timeout()), this, SLOT(timeHit()), Qt::DirectConnection);
    timer.setInterval(5000);
    timer.start();
    exec();
    timer.stop();
}

void ClockThread::timeHit()
{
    QDate currentDate = lastDate.addDays(1);
    if (lastDate != currentDate)
    {
        lastDate = currentDate;
        QString date_string = currentDate.toString("yyyy年MM月dd日");
        emit sendTime(date_string);
        QFile f("log.txt");
        if (!f.exists())
            f.open(QFile::WriteOnly);
        else
            f.open(QFile::Append);
        QTextStream stream(&f);
        stream << date_string << endl << endl;
        f.close();
    }
}

ClockThread::ClockThread()
{
    lastDate = QDate::currentDate();
}
