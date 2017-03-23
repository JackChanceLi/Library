#ifndef HEADER_H
#define HEADER_H

#include<QString>



//用户的一条借阅记录
class Record{
    //书名字
    //书详情指针
    //借书日期
    //借书时长
    //还书日期
    //
};



//用户的详细信息
class UserInfo{
public:
    //借阅图书记录
    //账户余额
};


class UserBasic{
public:
    QString account;
    QString password;
    QString username;
    QString number;
    QString academy;
    QString identity;
    int credit;

    UserBasic();

};



#endif // HEADER_H
