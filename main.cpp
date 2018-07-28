#include "header.h"
#include "logindialog.h"
#include "mainwindow.h"
#include "QFile"
#include "QTextStream"
#include"QDebug"
#include"register.h"

QDate last_date;//上次模拟终止运行的时间
QString manager_code;//管理员注册邀请码
QString logfile_name;//日志文件名
QString theme;//主题名称
int time_length;//时间步长
int seqinday;//图书的流水号

/**
 * @brief 软件入口
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //打开系统配置文件，读取相关系统参数
    QFile f("config.txt");
    f.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream(&f);
    QString date_str = stream.readLine().trimmed();
    bool ok = true;
    seqinday = stream.readLine().trimmed().toInt(&ok,10);//流水号
    manager_code = stream.readLine().trimmed();//取出尾部的回车\n
    logfile_name = stream.readLine().trimmed();
    theme = stream.readLine().trimmed();
    time_length = stream.readLine().trimmed().toInt(&ok,10);//将字符串转化为int整形
    qDebug()<<"系统起始时间为："<<date_str;
    qDebug()<<"流水号为："<<seqinday;
    qDebug()<<"管理员邀请码为："<<manager_code;
    qDebug()<<"日志文件名为："<<logfile_name;
    qDebug()<<"时间步长为："<<time_length;
    f.close();

    last_date = QDate::fromString(date_str, "yyyy MM dd");//将时间字符串转换为date变量

    qDebug()<<"系统起始日期："<<last_date;



    //启动登陆界面
    LoginDialog * dlg = new LoginDialog(last_date);
    dlg->show();
    a.exec();
    qDebug()<<"qid ";
/*
    //退出系统前保存配置信息
    f.open(QFile::ReadWrite | QFile::Text);
    QTextStream infile(&f);

    infile<<QString::number(last_date.year(),10)+" "
         <<last_date.month()<<" "
        <<last_date.day()<<endl;
    infile<<manager_code<<endl;
    infile<<logfile_name<<endl;
    infile<<theme<<endl;
    infile<<time_length<<endl;
    f.close();*/
    return 0;
}
