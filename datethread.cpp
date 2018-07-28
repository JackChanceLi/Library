#include "datethread.h"

extern QDate last_date;//日期时刻更新
extern int time_length;//时间步长
extern int seqinday;//流水号

/**
 * @brief 无参数构造函数
 * 以当前时间作为系统起始时间
 */
DateThread::DateThread()
{
    last_date = QDate::currentDate();

    stopped = false;//标识日期线程是否停止
}

/**
 * @brief 有参数构造函数
 * @param start_date
 */
DateThread::DateThread(QDate start_date)
{
    last_date = start_date;
    stopped = false;
}

void DateThread::stop()
{
    stopped = true;
}

void DateThread::on_one_second(int pro){
    emit send_progress(pro);
}

/**
 * @brief 日期线程函数
 */
void DateThread::run()
{
    QDate current_date = last_date;
    while (!stopped) {
        QString current_date_str = current_date.toString("yyyy年MM月dd日");
        emit send_date(current_date_str);
        if (last_date != current_date)
            last_date = current_date;//日期时刻更新
        seqinday = 0;//新的一天更新流水号

        //计时一天的时间
        for(int i=0;i<time_length*10;i++){
            msleep(100);
            emit send_progress(i+1);
        }

        current_date = current_date.addDays(1);
    }
    stopped = false;
}
