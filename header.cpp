#include"header.h"
extern QDate last_date;
User::User()
{
    account = "";
    password = "";
    username = "";
    number = "";
    academy = "";
    identity = "";
    gender = "";
    credit = 60;
    apartment = "";
    borrowlist = QList<BorrowRecord>();
}

QString User::get_account()
{
    return account;
}

void User::set_account(QString acc)
{
    account = acc;
}

int User::get_credit()
{
    return credit;
}

void User::set_credit(int cre)
{
    credit = cre;
}

QString User::get_identity()
{
    return identity;
}

void User::set_identity(QString id)
{
    identity = id;
}

QString User::get_username()
{
    return username;
}

void User::set_username(QString name)
{
    username = name;
}

QString User::get_password()
{
    return password;
}

void User::set_password(QString pass)
{
    password = pass;
}

QString User::get_picsrc()
{
    return picsrc;
}

void User::set_picsrc(QString psrc)
{
    picsrc = psrc;
}

QList<BorrowRecord> User::getBorrowRecord()
{
    return borrowlist;
}

void User::addBorrowRecord(BorrowRecord record)
{
    borrowlist.append(record);
}

void User::updateBorrowRecord(QList<BorrowRecord> recordlist)
{
    borrowlist = recordlist;
}

QList<OrderRecord> User::getOrderRecord()
{
    return orderlist;
}

void User::addOrderRecord(OrderRecord record)
{
    orderlist.append(record);
}

void User::updateOrderRecord(QList<OrderRecord> recordlist)
{
    orderlist = recordlist;
}

BorrowRecord::BorrowRecord()
{
}

QString BorrowRecord::get_spec_index()
{
    return spec_index;
}

void BorrowRecord::set_spec_index(QString in)
{
    spec_index = in;
}

int BorrowRecord::get_condition()
{
    return condition;
}

void BorrowRecord::set_condition(int c)
{
    condition = c;
}

QString BorrowRecord::get_bookfile_name()
{
    return bookfile_name;
}

void BorrowRecord::set_bookfile_name(QString name)
{
    bookfile_name = name;
}

QString BorrowRecord::get_borrow_date()
{
    return borrow_date;
}

void BorrowRecord::set_borrow_date(QString date)
{
    borrow_date = date;
}

int BorrowRecord::get_days_left()
{
    return days_left;
}

void BorrowRecord::set_days_left(int left)
{
    days_left = left;
}

QString BorrowRecord::get_return_date()
{
    return return_date;
}

void BorrowRecord::set_return_date(QString date)
{
    return_date = date;

}

OrderRecord::OrderRecord()
{
    spec_index = "";
    bookfile_name = "";
    days_left = INT_MAX;
    available_date = "";
}

QString OrderRecord::get_spec_index()
{
    return spec_index;
}

void OrderRecord::set_spec_index(QString in)
{
    spec_index = in;
}

int OrderRecord::get_condition()
{
    return condition;
}

void OrderRecord::set_condition(int c)
{
    condition = c;
}

QString OrderRecord::get_bookfile_name()
{
    return bookfile_name;
}

void OrderRecord::set_bookfile_name(QString name)
{
    bookfile_name = name;
}

int OrderRecord::get_days_left()
{
    return days_left;
}

void OrderRecord::set_days_left(int left)
{
    days_left = left;
}

QString OrderRecord::get_available_date()
{
    return available_date;
}

void OrderRecord::set_available_date(QString date)
{
    available_date = date;
}

/**
  * @brief 输出系统日志到日志文件
  * @param logfile //日志文件夹
  * @param Type //日志类型
  * @param user //日志操作者
  * @param message //日志内容
  */
 void  Tool::systemlog(QString logfile,QString Type, QString user, QString message){

    QFile file(logfile);
    file.open(QIODevice::ReadWrite | QIODevice::Text |QIODevice::Append);
    QTextStream log(&file);

    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString timestr = time.toString("yyyy-MM-dd hh:mm:ss ddd"); //设置显示格式
    QString str0 = QString("实际系统时间：");
    QString str1 = QString("模拟系统时间：");
    QString str2 = QString("事件：");
    QString str3 = QString("操作者：");
    QString str4 = QString("操作内容：");
    QString spilt = QString("******************新日志******************");
    log<<spilt<<endl;
    log<<str0<<timestr<<endl;
    log<<str1<<last_date.toString("yyyy-MM-dd")<<endl;
    log<<str2<<Type<<endl;
    log<<str3<<user<<endl;
    log<<str4<<message<<endl<<endl;
    file.close();
}

