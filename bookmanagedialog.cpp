#include "bookmanagedialog.h"
#include "ui_bookmanagedialog.h"
#include"QDir"
#include"QFileInfo"
#include"QMessageBox"
#include"QDebug"
#include"QString"

BookManageDialog::BookManageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BookManageDialog)
{
    ui->setupUi(this);

    fileinfo = new QList<QFileInfo>();
    update_booklist();


}

BookManageDialog::~BookManageDialog()
{
    delete ui;



}

/**
 * @brief 基于书名，打开对应的图书文件
 * @param filename
 */
void BookManageDialog::get_bookinfo(QString filename){
    //打开图书文件，读取图书详细信息
    //qDebug()<<filename;
    QFile f("./Books/" + filename);
    if (!f.open(QFile::ReadOnly))
    {
        QMessageBox::warning(this, "错误", "打开文件失败！");
        return;
    }
    QByteArray file_data = f.readAll();
    f.close();

    QJsonDocument file_doc(QJsonDocument::fromJson(file_data));
    QJsonObject file_obj = file_doc.object();
    QJsonObject record_obj = file_obj["record"].toObject();

    ui->Edit_amount->setText(QString::number(file_obj["amount"].toInt(),10));
    ui->Edit_author->setText(file_obj["author"].toString());
    ui->Edit_info->setText(file_obj["intro"].toString());
    ui->Edit_title->setText(filename.section('#',1,1));
    ui->Edit_index->setText(filename.section('#',0,0));
    ui->comboBox_department->setCurrentIndex(filename[0].digitValue());
    ui->comboBox_right->setCurrentIndex(filename[1].digitValue());
}

void BookManageDialog::on_list_book_currentRowChanged(int currentRow)
{
    get_bookinfo(fileinfo->at(currentRow).fileName());
}

/**
 * @brief 对书目信息进行更改
 */
void BookManageDialog::on_Button_confirm_clicked()
{
    if(QMessageBox::warning(this,"确认","是否确认更改？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes){
        QString filename = fileinfo->at(ui->list_book->currentIndex().row()).fileName();
        QFile f("./Books/" + filename);
        if (!f.open(QFile::ReadOnly))
        {
            QMessageBox::warning(this, "错误", "打开文件失败！");
            return;
        }
        QByteArray file_data = f.readAll();
        f.close();
        QJsonDocument file_doc(QJsonDocument::fromJson(file_data));
        QJsonObject file_obj = file_doc.object();
        file_obj["amount"] = ui->Edit_amount->text();
        file_obj["author"] = ui->Edit_author->text();


        QJsonDocument savefile(file_obj);
        QFile file;
        file.remove("./Books/" + filename);
        
        if(ui->comboBox_department->currentIndex() == 0){
            filename[0] = '0';
        }else if(ui->comboBox_department->currentIndex() == 1)
            filename[0] = '1';
        else if(ui->comboBox_department->currentIndex() == 2)
            filename[0] = '2';

        if(ui->comboBox_right->currentIndex() == 0){
            filename[1] = '0';
        }else if(ui->comboBox_right->currentIndex() == 1)
            filename[1] = '1';
        else if(ui->comboBox_right->currentIndex() == 2)
            filename[1] = '2';
        else if(ui->comboBox_right->currentIndex() == 3)
            filename[1] = '3';
        else if(ui->comboBox_right->currentIndex() == 4)
            filename[1] = '4';
        else if(ui->comboBox_right->currentIndex() == 5)
            filename[1] = '5';
        QString filename_new = filename.section('#',0,0)+"#"+ui->Edit_title->text()+"#"+filename.section('#',2,2)+"#";
        qDebug()<<"新文件名："<<filename_new;

        QFile f_("./Books/" + filename_new + ".json");
        f_.open(QIODevice::Truncate | QIODevice::WriteOnly);
        f_.write(savefile.toJson());
        f_.close();

        QMessageBox::information(this,"成功","图书信息修改成功","我知道了！");
        setVisible(false);
        close();
        BookManageDialog *bmd = new BookManageDialog();
        bmd->exec();
    }
}

/**
 * @brief 更新
 */
void BookManageDialog::update_booklist(){

    //读取所有的图书信息
    QDir dir;
    QDir abs_dir = dir.absolutePath();
    if (abs_dir.dirName() != "Books" && !abs_dir.exists("Books"))
    {
        QMessageBox::warning(this, "错误", "未连接到图书库！");
        dir.mkdir("Books");
    }
    dir.cd("Books");//cd到book目录下

    //设置过滤
    QStringList filter;
    filter<<"*.json";
    dir.setNameFilters(filter);
    fileinfo->clear();
    fileinfo->append(dir.entryInfoList(filter));

    QList<QString> booklist;
    for(int i=0; i < fileinfo->size();i++){
        QString str = fileinfo->at(i).fileName();
        QString title = str.section('#',1,1);
        qDebug()<<"读取图书:"<<title;
        booklist.append(title);
    }
    ui->list_book->addItems(booklist);
}

/**
 * @brief 删除图书
 */
void BookManageDialog::on_pushButton_delete_clicked()
{
    if(QMessageBox::warning(this,"确认","是否确认删除图书？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes){
        QString filename = fileinfo->at(ui->list_book->currentIndex().row()).fileName();
        QFile file;
        file.remove("./Books/" + filename);

        QMessageBox::information(this,"成功","删除成功！","我知道了");
        setVisible(false);
        close();
        BookManageDialog *bmd = new BookManageDialog();
        bmd->exec();
    }
}
