#include "register.h"
#include "ui_register.h"
#include<Qstring>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include<QFile>
#include<QDebug>
#include<QMessageBox>

/**
 * @brief Register::Register
 * @param parent
 * 这里实现注册界面的逻辑
 */
Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);

}


Register::~Register()
{
    delete ui;
}


void Register::on_Button_accept_clicked()
{
    ui->Button_reg->setText("注册");
}

void Register::on_Button_reject_clicked()
{
    ui->Button_reg->setText("退出");
}

void Register::on_Button_reg_clicked()
{
    if (ui->Button_reg->text() == "退出")
        close();
    else{
        //读取表单文件
        QString name = ui->lineEdit_name->text();
        QString account = ui->lineEdit_account->text();
        QString password = ui->lineEdit_key->text();
        QString identify = ui->combox_identify->currentText();

        //构建用户记录的Json对象
        QJsonObject user;
        user["name"] = name;
        user["account"] = account;
        user["password"] = password;
        user["identify"] = identify;

        //读入本地存储的用户数据
        QFile file;
        file.setFileName("user.json");
        if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){//文件读取失败
            QMessageBox::warning(this, "错误", "丢失用户文件!","我知道了");
            return;
        }
        QString val = file.readAll();
        file.close();


        //解析json内容
        QJsonDocument value = QJsonDocument::fromJson(val.toUtf8());
        QJsonObject data = value.object();
       // qDebug()<<data;
        QJsonArray userlist = data.value("userlist").toArray();
        userlist.insert(0,user);

        data.remove("userlist");
        data.insert("userlist",userlist);
        //qDebug()<<data;

        if(QMessageBox::warning(this, "警告", "是否为"+name+"注册信息?",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes){
            QJsonDocument savefile(data);
            file.setFileName("user.json");
            file.open(QIODevice::Truncate|QIODevice::WriteOnly);
            file.write(savefile.toJson());
            file.close();
        }


    }


}
