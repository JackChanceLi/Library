#include "logindialog.h"
#include "ui_logindialog.h"
#include "header.h"
#include"register.h"
#include<QDebug>
#include<QSettings>
#include<QJsonDocument>
#include<QJsonObject>
#include<QFile>
#include<QJsonArray>
#include<QList>
#include"header.h"

extern QList<UserBasic> rem_user;
UserBasic current_user;//当前用户信息

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    ui->user_account->setCurrentText(rem_user[0].account);
    ui->pwd_lineEdit->setText(rem_user[0].password);

    //装填下拉框
    for(int i=0;i<rem_user.count();i++){
        ui->user_account->addItem(rem_user[i].account);
    }

    connect(this, &LoginDialog::require_register_dialog, this, &LoginDialog::open_register_dialog);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

/**
 * @brief LoginDialog::on_login_pushButton_clicked
 * 实现登陆按钮的逻辑，检测账号，读取用户信息等
 */
void LoginDialog::on_login_pushButton_clicked()
{
    //进行账号检查
    //这里是要调取用户库的
    bool ok = false;

    //载入注册用户列表
    QFile file;
    file.setFileName("user.json");
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QString val = file.readAll();
    file.close();

    QJsonDocument value = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject data = value.object();
    QJsonArray userlist = data["userlist"].toArray();
    //qDebug()<<userlist;

    for(int i=0; i<userlist.count();i++){
        if(ui->user_account->currentText().toStdString() == userlist[i].toObject().value("account").toString().toStdString()
                && ui->pwd_lineEdit->text().toStdString() == userlist[i].toObject().value("password").toString().toStdString()){
           //qDebug()<<userlist[i].toObject().value("name").toString();
            ok = true;
            current_user.username = userlist[i].toObject().value("name").toString();
            current_user.account = userlist[i].toObject().value("account").toString();
            current_user.identity = userlist[i].toObject().value("identify").toString();
            current_user.password = userlist[i].toObject().value("password").toString();
            //qDebug()<<current_user.username;
            break;
        }
    }


    if (ok){
        //检查是否记录账号
        if(ui->remember_checkbox->checkState() == Qt::Checked){
            //检查是否已经保存
            for(int i=0;i<rem_user.count();i++){
                if(ui->user_account->currentText() == rem_user[i].account
                        && ui->pwd_lineEdit->text() == rem_user[i].password){
                    accept();
                    return;
                }
            }

            //没有保存，写进json文件
            QFile file;
            file.setFileName("rem.json");
            file.open(QIODevice::ReadWrite | QIODevice::Text);
            QString contain = file.readAll();
            file.close();
            QJsonDocument val = QJsonDocument::fromJson(contain.toUtf8());
            QJsonObject data = val.object();

            QJsonArray rem_user_list = data["rem_account"].toArray();

            QJsonObject info;
            info.insert("acc",ui->user_account->currentText());
            info.insert("pas",ui->pwd_lineEdit->text());
            rem_user_list.insert(0,info);

            data.remove("rem_account");
            data.insert("rem_account",rem_user_list);

            qDebug()<<data;

            file.setFileName("rem.json");
            file.open(QIODevice::Truncate|QIODevice::WriteOnly);
            QJsonDocument save_file(data);
            file.write(save_file.toJson());
            file.close();
        }
        accept();

    }else
    {
        QMessageBox::warning(this, "Warning!", "User name or password not correct!", QMessageBox::Yes);
        ui->user_account->clearEditText();
        ui->pwd_lineEdit->clear();
        ui->user_account->setFocus();
    }
}

/**
 * @brief LoginDialog::on_user_account_activated
 * @param index
 * 处理账户选择的更改部分
 */
void LoginDialog::on_user_account_activated(int index)
{
    ui->pwd_lineEdit->setText(rem_user[index].password);
}

/**
 * @brief LoginDialog::open_register_dialog
 * 打开注册界面
 */
void LoginDialog::open_register_dialog()
{
    Register * regdlg = new Register();
    regdlg->setWindowModality(Qt::ApplicationModal);
    regdlg->show();

}

void LoginDialog::on_register_pushButton_clicked()
{
    emit require_register_dialog();
}
