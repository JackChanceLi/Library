#include "logindialog.h"
#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include<QString>
#include"header.h"
#include<QJsonDocument>
#include<QJsonObject>
#include<QFile>
#include<QJsonArray>
#include<QList>

QList<UserBasic> rem_user;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //打开Json文件
    QFile file;
    file.setFileName("rem.json");
    file.open(QIODevice::ReadWrite | QIODevice::Text);

    //提取Json内容
    QString val;
    val = file.readAll();
    file.close();

    //从string中提取data数据
    QJsonDocument data = QJsonDocument::fromJson(val.toUtf8());
    //最外层是一个Json对象

    QJsonObject rem = data.object();
    QJsonArray account = rem["rem_account"].toArray();

    for(int i=0;i<account.count();i++){
        QJsonObject acc = account[i].toObject();
        UserBasic temp;
        temp.account = acc.value("acc").toString();
        temp.password = acc.value("pas").toString();
       // qDebug()<<temp.account<<" and "<<temp.password;
        rem_user.append(temp);
       // qDebug()<<rem_user[0].account;
    }


    LoginDialog dlg;


    if (dlg.exec() == QDialog::Accepted)
    {
        MainWindow w;
        w.show();
        return a.exec();
    }
    else return 0;
}
