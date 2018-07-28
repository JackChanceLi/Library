#include "header.h"
#include "register.h"
#include "ui_register.h"


extern QString manager_code;
extern QString logfile_name;


Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    //getin
        //设置标题栏不可见
        this->setWindowFlags(Qt::FramelessWindowHint);
        this->setAttribute(Qt::WA_TranslucentBackground);

        //背景图片


    //getend
    ui->setupUi(this);
    ui->combox_identify->setCurrentIndex(0);
    ui->groupBox_admin->hide();
    picsrc = "头像.png";
    QPixmap pix(picsrc);
    ui->label_pic->setPixmap(pix);
    ui->label_pic->setScaledContents(true);
    ui->label_pic->show();
    ui->lineEdit_name->setFocus();

    QToolButton *closeButton= new QToolButton(this);

    QPixmap closePix;
    closePix.load(":/background/resourse/X.png");

    closeButton->setIcon(closePix);

    closeButton->setGeometry(588,5,20,20);
    closeButton->setToolTip(tr("关闭"));
    closeButton->setStyleSheet("QToolButton{background-color:rgb(255,255,255,0,0);border-radius:4px;background-image:url(:/sdfa.png)}"
                               "QToolButton:hover{background-color:rgb(245,31,31);}");

    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
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
    else
    {
        //读取表单文件
        name = ui->lineEdit_name->text();
        account = ui->lineEdit_account->text();
        password = ui->lineEdit_key->text();
        identify = ui->combox_identify->currentText();
        gender = ui->comboBox_gender->currentText();

        //构建用户记录的Json对象
        QJsonObject user;
        user["account"] = account;
        user["password"] = password;
        user["name"] = name;
        user["gender"] = gender;
        user["identify"] = identify;
        user["credit"] = 60;  //默认信用度60分
        user["picsrc"] = picsrc;
        QJsonObject borrow_records;
        user["borrow_records"] = borrow_records;
        QJsonObject order_records;
        user["order_records"] = order_records;


        if(identify == "学生" || identify == "教职工")
        {
            stu_number = ui->lineEdit_number->text();
            stu_class = ui->lineEdit_class->text();
            stu_enroll = ui->dateTimeEdit_regyear->date().year();
            stu_academy = ui->comboBox_college->currentText();
            user["number"] = stu_number;          
            user["apartment"] = stu_class;
            user["enroll"] = stu_enroll;
            user["academy"] = stu_academy;
        }
        else
        {
            admin_number = ui->lineEdit_number_admin->text();
            admin_code = ui->lineEdit_code_admin->text();
            admin_apartment = ui->comboBox_college_admin->currentText();
            if(admin_code != manager_code)
            {
                QMessageBox::warning(this, "验证失败", "邀请码错误，请核实上级给您的邀请码!", "我知道了");
                ui->lineEdit_code_admin->setFocus();
                return;
            }
            user["number"] = admin_number;
            user["apartment"] = admin_apartment;
            user["enroll"] = "";
            user["academy"] = "";
        }

        //读入本地存储的用户数据
        QFile file("user.json");
        QByteArray val;
        if(file.open(QFile::ReadOnly))
        {
            val = file.readAll();
            file.close();
        }

        QJsonDocument file_doc(QJsonDocument::fromJson(val));
        QJsonObject user_data = file_doc.object();

        QJsonArray userlist = user_data["userlist"].toArray();
        userlist.append(user);
        user_data.remove("userlist");
        user_data.insert("userlist",userlist);

        if(QMessageBox::warning(this, "警告", "是否确认注册?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
        {
            QJsonDocument savefile(user_data);
            QFile f_("user.json");
            f_.open(QIODevice::Truncate | QIODevice::WriteOnly);
            f_.write(savefile.toJson());
            f_.close();
            QMessageBox::warning(this, "成功", "注册成功，现在可以使用账户和密码登陆了!", "我知道了!");
            Tool::systemlog(logfile_name,"新用户注册",name,"新用户注册成功！");
        }
    }
    close();
}



void Register::on_combox_identify_currentIndexChanged(int index)
{
    if (index == 0)
    {
        ui->groupBox_admin->hide();
        ui->groupBox_user->show();
        ui->lineEdit_class->setDisabled(false);
        ui->label_class->setDisabled(false);
        ui->label_index->setText("*学号");
        ui->label_regyear->setText("*入校时间");
    }
    if (index == 1)
    {
        ui->groupBox_user->hide();
        ui->groupBox_admin->show();
    }
    if (index == 2)
    {
        ui->groupBox_admin->hide();
        ui->groupBox_user->show();
        ui->lineEdit_class->setDisabled(true);
        ui->label_index->setText("*工号");
        ui->label_class->setDisabled(true);
        ui->label_regyear->setText("*入职时间");
    }
}



void Register::on_toolButton_clicked()
{
    if((picsrc = QFileDialog::getOpenFileName(this, "上传照片", NULL, "Image(*.jpg *.png)")) != "")
    {
        QPixmap pix(picsrc);
        ui->label_pic->setPixmap(pix);
        ui->label_pic->setScaledContents(true);
        ui->label_pic->show();

    }
}

void Register::mousePressEvent(QMouseEvent * event)
{
    if(event->button()==Qt::LeftButton)
    {
        m_Drag = true;
        m_DragPosition = event->globalPos() - this->pos();
        event->accept();
    }
}

void Register::mouseMoveEvent(QMouseEvent *event)
{
    if(m_Drag && (event->buttons() && Qt::LeftButton)){
        move(event->globalPos() - m_DragPosition);
        event->accept();
    }
//my_thread->stop();
}

void Register::mouseReleaseEvent(QMouseEvent *)
{
    m_Drag = false;
}
