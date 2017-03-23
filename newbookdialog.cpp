#include "newbookdialog.h"
#include "ui_newbookdialog.h"
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>


/**
 * @brief NewBookDialog::NewBookDialog
 * @param parent
 * 新书入库界面，
 */
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

/**
 * @brief NewBookDialog::on_Button_save_quit_clicked
 * 保存并退出按钮逻辑
 */
void NewBookDialog::on_Button_save_quit_clicked()
{
    //容错判断
    if (QMessageBox::warning(this, "警告", "是否确定保存？", QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
        return;

    //文件夹类
    QDir dir;

    dir.makeAbsolute();

    QMessageBox::warning(this, "当前路径", dir.currentPath() + "," + dir.dirName());

    if(dir.dirName() == "Books")
        QMessageBox::warning(this, "嗯", "已经在这个目录里了");

    if (dir.dirName() != "Books" && !dir.exists("Books"))
    {
        dir.mkdir("Books");//创建文件夹
        if (dir.mkdir("Books"))
        {
            QMessageBox::warning(this, "嗯", "创建好Books文件夹了");
            QMessageBox::warning(this, "en", dir.currentPath());
        }
        else
        {
            QMessageBox::warning(this, "嗯", "无法创建");
            return;
        }
    }
    //当前位置
    dir.setCurrent("./Books");



    //读取表单信息
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


    //Json格式
    QJsonObject ABook;
    ABook["author"] = author;
    ABook["publisher"] = publisher;
    ABook["amount"] = amount;
    ABook["intro"] = intro;

    //内置Json对象，记录图书借阅记录
    QJsonObject record;
    for (int i = 0; i < amount; i++)
    {
        QString index = general_index + "#" + QString::number(i);
        record[index] = true;
    }
    ABook["record"] = record;


    //打开保存文件，创建。。。
    QFile f(general_index + " " + title + " " + author + ".json");
    if (!f.open(QFile::WriteOnly))
    {
        QMessageBox::warning(this, "err", "cant open");
        return;
    }

    //保存JSON
    QJsonDocument save_file(ABook);
    f.write(save_file.toJson());


    QMessageBox::warning(this, "嗯", "图书条目保存成功！");

    dir.cdUp();

    close();
}


/**
 * @brief NewBookDialog::on_Button_save_next_clicked
 * 保存并添加下一本书
 */
void NewBookDialog::on_Button_save_next_clicked()
{
    if (QMessageBox::warning(this, "警告", "是否确定保存？", QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
        return;

    //寻文件夹
    QDir dir;
    dir.makeAbsolute();
    if (dir.dirName() != "Books" && !dir.exists("Books"))
    {
        dir.mkdir("Books");
    }
    dir.setCurrent("./Books");


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
    ABook["author"] = author;
    ABook["publisher"] = publisher;
    ABook["amount"] = amount;
    ABook["intro"] = intro;
    QJsonObject record;
    for (int i = 0; i < amount; i++)
    {
        QString index = general_index + "#" + QString::number(i);
        record[index] = true;
    }
    ABook["record"] = record;

    QFile f(general_index + " " + title + " " + author + ".json");
    if (!f.open(QFile::WriteOnly))
    {
        QMessageBox::warning(this, "err", "cant open");
        return;
    }
    QJsonDocument save_file(ABook);
    f.write(save_file.toJson());
    f.close();


    QMessageBox::warning(this, "嗯", "图书条目保存成功！");

    ui->Edit_Title->clear();
    ui->Edit_Author->clear();
    ui->Edit_Publisher->clear();
    ui->Edit_Amount->clear();
    ui->Edit_Intro->clear();
    ui->Edit_Title->setFocus();

//    if (!dir.cdUp())
//        QMessageBox::warning(this, "cant change current path", dir.currentPath());
}
