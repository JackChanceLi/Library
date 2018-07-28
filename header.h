#ifndef HEADER_H
#define HEADER_H

#include <QString>
#include <QSettings>
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QDate>
#include <QCursor>
#include <QFileDialog>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QComboBox>
#include <QList>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QPixmap>
#include <QDebug>
#include <QMouseEvent>
#include <QMovie>
#include <QLabel>
#include <QToolBar>
#include <QToolButton>
#include <QStatusBar>
#include <QLCDNumber>

class BorrowRecord
{

private:
    QString spec_index;
    int condition;
    QString bookfile_name;
    QString borrow_date;
    int days_left;
    QString return_date;

public:

    BorrowRecord();

    QString get_spec_index();
    void set_spec_index(QString in);

    int get_condition();
    void set_condition(int c);

    QString get_bookfile_name();
    void set_bookfile_name(QString name);

    QString get_borrow_date();
    void set_borrow_date(QString date);

    int get_days_left();
    void set_days_left(int left);

    QString get_return_date();
    void set_return_date(QString date);

};

class OrderRecord
{

private:
    QString spec_index;
    int condition;
    QString bookfile_name;
    int days_left;
    QString available_date;

public:

    OrderRecord();

    QString get_spec_index();
    void set_spec_index(QString in);

    int get_condition();
    void set_condition(int c);

    QString get_bookfile_name();
    void set_bookfile_name(QString name);

    int get_days_left();
    void set_days_left(int left);

    QString get_available_date();
    void set_available_date(QString date);

};

class User
{

private:
    QString account;
    int credit;
    QString identity;
    QString username;
    QString password;
    QString picsrc;
    QList<BorrowRecord> borrowlist;
    QList<OrderRecord> orderlist;

public:
    QString apartment;
    QString academy;
    QString gender;
    QString number;
    QString enroll;
    User();

    QString get_account();
    void set_account(QString acc);

    int get_credit();
    void set_credit(int cre);

    QString get_identity();
    void set_identity(QString id);

    QString get_username();
    void set_username(QString name);

    QString get_password();
    void set_password(QString pass);

    QString get_picsrc();
    void set_picsrc(QString psrc);

    QList<BorrowRecord> getBorrowRecord();
    void addBorrowRecord(BorrowRecord record);
    void updateBorrowRecord(QList<BorrowRecord> recordlist);

    QList<OrderRecord> getOrderRecord();
    void addOrderRecord(OrderRecord record);
    void updateOrderRecord(QList<OrderRecord> recordlist);

};

/**
 * @brief 日志输出工具类
 */
class Tool{
public:
    static void  systemlog(QString logfile,QString Type, QString user,QString message);
};

#endif // HEADER_H
