#include "dialogfine.h"
#include "ui_dialogfine.h"
#include"header.h"

extern User current_user;
extern QString logfile_name;

DialogFine::DialogFine(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFine)
{
    ui->setupUi(this);
    ui->label_name->setText(current_user.get_username());
    ui->label_credit->setText(QString::number(current_user.get_credit(),10));
    ui->label_fine->setText(QString::number(60 - current_user.get_credit(),10)+"￥");

}

DialogFine::~DialogFine()
{
    delete ui;
}

/**
 * 点击支付按钮
 */
void DialogFine::on_pushButton_payok_clicked()
{
     if(ui->lineEdit_password->text().isEmpty())
        QMessageBox::warning(this,"提示","请输入校园卡密码","我知道了");
     else
     {
        if(QMessageBox::warning(this, "警告", "是否确认支付？",
                                QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes){
         //检查密码是否正确
         if(ui->lineEdit_password->text() != current_user.get_password()){
             QMessageBox::warning(this,"错误","抱歉，您的密码输入有误，请重新输入！","确定");
         }else if(ui->lineEdit_password->text() == current_user.get_password()){
              QMessageBox::warning(this,"支付成功","您好，您的罚款已经支付成功，现在您可以继续借书了！","我知道了！");
              Tool::systemlog(logfile_name,"支付成功",current_user.get_username(),"缴纳罚款成功！");
              current_user.set_credit(60);
              close();
            }
        }
    }
}


/**
 * 取消支付
 */
void DialogFine::on_pushButton_payquit_clicked()
{
    close();
}
