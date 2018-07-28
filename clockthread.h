#ifndef CLOCKTHREAD_H
#define CLOCKTHREAD_H

#include <QThread>
#include <QObject>
#include <QTimer>
#include <QDate>
#include <QFile>
#include <QTextStream>

class ClockThread : public QThread
{
    Q_OBJECT
signals:
    void sendTime(QString time);
private:
    void run();
    QDate lastDate;
private slots:
    void timeHit();
public:
    ClockThread();
};

#endif // CLOCKTHREAD_H
