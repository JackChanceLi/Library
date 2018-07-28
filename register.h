#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>

namespace Ui {
class Register;
}

class Register : public QDialog
{
    Q_OBJECT

public:
    bool m_Drag;
    QPoint m_DragPosition;
    explicit Register(QWidget *parent = 0);
    ~Register();
    //基本信息
    QString name;
    QString account;
    QString password;
    QString identify;
    QString gender;
    QString picsrc;

    //学生信息
    QString stu_number;
    QString stu_class;
    int stu_enroll;
    QString stu_academy;

    //职员信息
    QString admin_number;
    QString admin_code;
    QString admin_apartment;



private slots:

    void on_Button_accept_clicked();

    void on_Button_reject_clicked();

    void on_Button_reg_clicked();

    void on_combox_identify_currentIndexChanged(int index);


    void on_toolButton_clicked();


    void mousePressEvent(QMouseEvent * event);

    void mouseMoveEvent(QMouseEvent * event);

    void mouseReleaseEvent(QMouseEvent *);

private:
    Ui::Register *ui;
};

#endif // REGISTER_H
