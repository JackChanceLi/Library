#include "header.h"
#include "newbookdialog.h"
#include "ui_newbookdialog.h"
extern User current_user;
extern QString logfile_name;
extern int seqinday;

NewBookDialog::NewBookDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewBookDialog)
{
    ui->setupUi(this);
    ui->Edit_Title->setFocus();

}

NewBookDialog::~NewBookDialog()
{
    delete ui;
}

void NewBookDialog::on_Button_save_quit_clicked()
{
    if (QMessageBox::warning(this, "警告", "是否确定保存？", QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
        return;

    QDir dir;

    QDir abs_dir = dir.absolutePath();

    if (abs_dir.dirName() != "Books" && !abs_dir.exists("Books"))
    {
        dir.mkdir("Books");//创建文件夹
        if (dir.mkdir("Books"))
        {
            QMessageBox::warning(this, "嗯", "创建好Books文件夹了");
        }
        else
        {
            QMessageBox::warning(this, "嗯", "无法创建");
            return;
        }
    }
    dir.cd("Books");

    QString title = ui->Edit_Title->text();
    QString author = ui->Edit_Author->text();
    QString publisher = ui->Edit_Publisher->text();
    QString inDate = ui->Edit_Indate->date().toString("yyMMdd");
    QString intro = ui->Edit_Intro->toPlainText();
    int department_index = ui->Edit_Department->currentIndex();
    int permission_index = ui->Edit_Permission->currentIndex();
    int amount = ui->Edit_Amount->text().toInt();
    //通用图书编号：图书部门编号(1位) + 图书权限编号(1位) + 购入时间(六位年月日) + 流水号(当日第几批新入的书，从日志获得最新的流水号) + #(用于分隔) + 当前第几本
    QString general_index = QString::number(department_index)
            + QString::number(permission_index)
            + QString(inDate);
    if(seqinday < 10)
        general_index = general_index + "00";
    else if(seqinday <100)
        general_index = general_index + "0";
    general_index = general_index + QString::number(seqinday,10);
    qDebug()<<"新书的编号为："<<general_index;
    seqinday++;

    QJsonObject ABook;
    ABook["author"] = author;
    ABook["publisher"] = publisher;
    ABook["amount"] = amount;
    ABook["intro"] = intro;

    //为0：可借状态；为1，借出可预约状态；为2，借出已预约状态。
    QJsonObject record;
    for (int i = 0; i < amount; i++)
    {
        QString index = general_index + "#" + QString::number(i);
        QJsonObject status;
        status["condition"] = 0;
        status["days_left"] = 0;
        status["current_owner"] = "";
        status["order_user"] = "";
        record[index] = status;
    }
    ABook["record"] = record;


    QFile f("./Books/" + general_index + "#" + title + "#" + author + "#.json");
    if (!f.open(QFile::WriteOnly))
    {
        QMessageBox::warning(this, "err", "cant open");
        return;
    }


    QJsonDocument save_file(ABook);
    f.write(save_file.toJson());
    f.close();

    Tool::systemlog(logfile_name,"新书录入",current_user.get_username(),"录入新书--《"+title+"》");
    QMessageBox::warning(this, "嗯", "图书条目保存成功！");

    close();
}

void NewBookDialog::on_Button_save_next_clicked()
{
    if (QMessageBox::warning(this, "警告", "是否确定保存？", QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
        return;

    QDir dir;
    QDir abs_dir = dir.absolutePath();
    if (abs_dir.dirName() != "Books" && !abs_dir.exists("Books"))
    {
        dir.mkdir("Books");
    }

    QString title = ui->Edit_Title->text();
    QString author = ui->Edit_Author->text();
    QString publisher = ui->Edit_Publisher->text();
    QString inDate = ui->Edit_Indate->date().toString("yyMMdd");
    QString intro = ui->Edit_Intro->toPlainText();
    int department_index = ui->Edit_Department->currentIndex();
    int permission_index = ui->Edit_Permission->currentIndex();
    int amount = ui->Edit_Amount->text().toInt();
    //通用图书编号：图书部门编号(1位) + 图书权限编号(1位) + 购入时间(六位年月日) + 流水号(当日第几批新入的书，从日志获得最新的流水号) + #(用于分隔) + 当前第几本
    QString general_index = QString::number(department_index)
            + QString::number(permission_index)
            + QString(inDate);
//            + (QString)serial_number;

    QJsonObject ABook;
    //ABook["title"]
    ABook["author"] = author;
    ABook["publisher"] = publisher;
    ABook["amount"] = amount;
    ABook["intro"] = intro;
    QJsonObject record;
    for (int i = 0; i < amount; i++)
    {
        QString index = general_index + "#" + QString::number(i);
        QJsonObject status;
        status["condition"] = 0;
        status["days_left"] = 0;
        status["current_owner"] = "";
        status["order_user"] = "";
        record[index] = status;
    }
    ABook["record"] = record;


    QFile f("./Books/" + general_index + "#" + title + "#" + author + "#.json");
    if (!f.open(QFile::WriteOnly))
    {
        QMessageBox::warning(this, "err", "cant open");
        return;
    }
    QJsonDocument save_file(ABook);
    f.write(save_file.toJson());
    f.close();

//  QMessageBox::warning(this, "嗯", dir.currentPath());


    QMessageBox::warning(this, "嗯", "图书条目保存成功！");
    Tool::systemlog(logfile_name,"新书录入",current_user.get_username(),"录入新书--《"+title+"》");

    ui->Edit_Title->clear();
    ui->Edit_Author->clear();
    ui->Edit_Publisher->clear();
    ui->Edit_Amount->clear();
    ui->Edit_Intro->clear();
    ui->Edit_Title->setFocus();

}
