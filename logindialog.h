#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "datethread.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QDate date, QWidget *parent = 0);
    ~LoginDialog();
    bool m_Drag;
    QPoint m_DragPosition;

signals:
    void require_register_dialog();

//protected:
//    bool eventFilter(QObject *obj, QEvent *event);

private slots:

    void change_date(QString date);

    void open_register_dialog();

    void on_login_pushButton_clicked();

    void on_user_account_activated(int index);

    void on_register_pushButton_clicked();

    void mousePressEvent(QMouseEvent * event);

    void mouseMoveEvent(QMouseEvent * event);

    void mouseReleaseEvent(QMouseEvent *);

    void update_progress(int pro);

private:
    Ui::LoginDialog *ui;
    DateThread * my_thread;
};

#endif // LOGINDIALOG_H
