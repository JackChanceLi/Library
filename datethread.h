#ifndef DATETHREAD_H
#define DATETHREAD_H

#include <QObject>
#include <QThread>
#include <QDate>

class DateThread : public QThread
{
    Q_OBJECT
signals:
    send_date(QString date);
    send_progress(int pro);

private slots:
    void on_one_second(int pro);

public:
    DateThread();
    DateThread(QDate start_date);
    void stop();
protected:
    void run();
private:
    volatile bool stopped;
    //QDate last_date;
};

#endif // DATETHREAD_H
