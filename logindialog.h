#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

signals:
    void require_register_dialog();


private slots:

    void on_login_pushButton_clicked();//登陆按钮

    void on_user_account_activated(int index);

    void on_register_pushButton_clicked();

    void open_register_dialog();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
