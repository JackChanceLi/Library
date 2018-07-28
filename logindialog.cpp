#include "header.h"
#include "logindialog.h"
#include "ui_logindialog.h"
#include "register.h"
#include "mainwindow.h"
#include <QDesktopWidget>


QList<User> rem_user;//记住密码列表
User current_user;//当前用户信息
extern QString logfile_name;//输出日志文件名
extern int time_length;
extern QDate last_date;

/**
 * @brief 构造函数
 * 参数为date
 */
LoginDialog::LoginDialog(QDate date, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{

    my_thread = new DateThread(date);//日期线程
    connect(my_thread, SIGNAL(send_date(QString)), this, SLOT(change_date(QString)));//接收到send之后对日期进行更改
    my_thread->start();
    ui->setupUi(this);

    //getin
        //设置标题栏不可见
        this->setWindowFlags(Qt::FramelessWindowHint);
        this->setAttribute(Qt::WA_TranslucentBackground);

        //背景图片
        QMovie *movie = new QMovie(":/background/resourse/login_1.png");
        ui->label_movie->setMovie(movie);
        movie->start();


        QToolButton *closeButton= new QToolButton(this);

        QPixmap closePix;

        closePix.load(":/background/resourse/X.png");

        closeButton->setIcon(closePix);

        closeButton->setGeometry(569,15,24,24);

        closeButton->setToolTip(tr("关闭"));

        closeButton->setStyleSheet("QToolButton{background-color:rgb(255,255,255,0,0);border-radius:4px;}"
                                   "QToolButton:hover{background-color:rgb(245,31,31);}");

        connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    //getend

    //读取记住密码的用户列表
    QFile file("rem.json");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString val;
        val = file.readAll();
        file.close();

        QJsonDocument data = QJsonDocument::fromJson(val.toUtf8());
        QJsonObject rem = data.object();
        QJsonArray account = rem["rem_account"].toArray();

        //将列表添加到下拉框
        rem_user.clear();
        for(int i=0;i<account.count();i++){
            QJsonObject acc = account[i].toObject();
            User temp;
            temp.set_account(acc.value("acc").toString());
            temp.set_password(acc.value("pas").toString());
            rem_user.append(temp);
        }

        //装填下拉框
        ui->user_account->clear();
        for(int i=0;i<rem_user.count();i++){
            ui->user_account->addItem(rem_user[i].get_account());
        }
        //默认选择第一个记住的账户
        ui->user_account->setCurrentText(rem_user[0].get_account());
        ui->user_password->setText(rem_user[0].get_password());
    }

    //打开注册界面的信号与槽
    connect(this, &LoginDialog::require_register_dialog, this, &LoginDialog::open_register_dialog);
    connect(my_thread,SIGNAL(send_progress(int)),this,SLOT(update_progress(int)));

}

/**
 * @brief 析构函数
 */
LoginDialog::~LoginDialog()
{
    delete ui;
}

/**
 * 注册按钮模块
 */
void LoginDialog::open_register_dialog()
{
    Register * regdlg = new Register();
    regdlg->exec();
}


/**
 * @brief 更新模拟系统现实的日期
 * @param date 日期
 */
void LoginDialog::change_date(QString date)
{
    ui->label_date->setText(date);
}



/**
 * @brief 登陆按钮的逻辑，验证账号
 */
void LoginDialog::on_login_pushButton_clicked()
{

    bool ok = false;//默认未验证
    current_user = User();//当前用户变量
    Tool::systemlog(logfile_name,"请求登陆",ui->user_account->currentText()+"（账号）","发起登陆请求");

    //载入注册用户列表
    QFile file;
    file.setFileName("user.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString val = file.readAll();
    file.close();

    QJsonDocument value = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject data = value.object();
    QJsonArray userlist = data["userlist"].toArray();

    //检查用户列表,并载入信息
    for(int i=0; i < userlist.count(); i++)
    {
        QJsonObject AUser = userlist[i].toObject();
        if(ui->user_account->currentText() == AUser["account"].toString()
                && ui->user_password->text() == AUser["password"].toString())
        {
            current_user.set_username(AUser["name"].toString());
            current_user.set_account(AUser["account"].toString());
            current_user.set_identity(AUser["identify"].toString());
            current_user.set_password(AUser["password"].toString());
            current_user.set_picsrc(AUser["picsrc"].toString());
            current_user.number = AUser["number"].toString();
            current_user.gender = AUser["gender"].toString();
            current_user.academy = AUser["academy"].toString();
            current_user.apartment = AUser["apartment"].toString();
            current_user.set_credit(AUser["credit"].toInt());
            QJsonObject borrow_records = AUser["borrow_records"].toObject();
            QJsonObject::const_iterator iter = borrow_records.constBegin();
            while (iter != borrow_records.constEnd())
            {
                BorrowRecord ABorrowRecord;
                ABorrowRecord.set_spec_index(iter.key());
                ABorrowRecord.set_condition(iter.value().toObject()["condition"].toInt());
                ABorrowRecord.set_borrow_date(iter.value().toObject()["borrow_date"].toString());
                ABorrowRecord.set_return_date(iter.value().toObject()["return_date"].toString());
                ABorrowRecord.set_bookfile_name(iter.value().toObject()["bookfile_name"].toString());
                ABorrowRecord.set_days_left(iter.value().toObject()["days_left"].toInt());
                current_user.addBorrowRecord(ABorrowRecord);
                iter++;
            }
            QJsonObject order_records = AUser["order_records"].toObject();
            QJsonObject::const_iterator iter_ = order_records.constBegin();
            while (iter_ != order_records.constEnd())
            {
                OrderRecord AnOrderRecord;
                AnOrderRecord.set_spec_index(iter_.key());
                AnOrderRecord.set_condition(iter_.value().toObject()["condition"].toInt());
                AnOrderRecord.set_available_date(iter_.value().toObject()["available_date"].toString());
                AnOrderRecord.set_bookfile_name(iter_.value().toObject()["bookfile_name"].toString());
                AnOrderRecord.set_days_left(iter_.value().toObject()["days_left"].toInt());
                current_user.addOrderRecord(AnOrderRecord);
                iter_++;
            }
            ok = true;
            break;
        }
    }//end of for

    if (ok)//验证通过
    {
        my_thread->stop();//时间线程停止
        Tool::systemlog(logfile_name,"账户验证",current_user.get_username(),"成功登陆系统");
        if (ui->remember_checkbox->checkState() == Qt::Checked) //检查是否需要记录账号
        {
            for (int i = 0; i < rem_user.count(); i++)
            {
                if (ui->user_account->currentText() == rem_user[i].get_account()
                        && ui->user_password->text() == rem_user[i].get_password()) { accept(); return; }
            }

            //向记录中添加新记录
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
            info.insert("pas",ui->user_password->text());
            rem_user_list.insert(0,info);
            data.remove("rem_account");
            data.insert("rem_account",rem_user_list);

            file.setFileName("rem.json");
            file.open(QIODevice::Truncate|QIODevice::WriteOnly);
            QJsonDocument save_file(data);
            file.write(save_file.toJson());
            file.close();
        }

        //进入主界面
        QDate current_date = QDate::fromString(ui->label_date->text(), "yyyy年MM月dd日");
        MainWindow * w = new MainWindow(current_date);
        w->move ((QApplication::desktop()->width() - w->width())/2,0);
        w->show();
        close();
    }
    else
    {
        Tool::systemlog(logfile_name,"账户验证",current_user.get_username(),"登陆系统失败!");
        QMessageBox::warning(this, "错误", "用户名或密码错误，请重新输入！", QMessageBox::Yes);
        ui->user_account->clearEditText();
        ui->user_password->clear();
        ui->user_account->setFocus();
    }
}


/**
 * @brief 登陆账户选择
 * @param index
 */
void LoginDialog::on_user_account_activated(int index)
{
    qDebug() << "user account activated";
    ui->user_password->setText(rem_user[index].get_password());
    my_thread->stop();
}

/**
 * @brief 点击注册按钮
 */
void LoginDialog::on_register_pushButton_clicked()
{
    qDebug() << "register button clicked";
    my_thread->stop();
    emit require_register_dialog();
}


void LoginDialog::mousePressEvent(QMouseEvent * event)
{
    if(event->button()==Qt::LeftButton)
    {
        m_Drag = true;
        m_DragPosition = event->globalPos() - this->pos();
        event->accept();
    }
}

void LoginDialog::mouseMoveEvent(QMouseEvent *event)
{
    if(m_Drag && (event->buttons() && Qt::LeftButton)){
        move(event->globalPos() - m_DragPosition);
        event->accept();
    }
//my_thread->stop();
}

void LoginDialog::mouseReleaseEvent(QMouseEvent *)
{
    m_Drag = false;
}

void LoginDialog::update_progress(int pro){
    ui->progressBar_time->setMaximum(100);
    ui->progressBar_time->setMinimum(0);
    ui->progressBar_time->setValue(pro*100/(time_length*10-1));
}
