#include "searchdialog.h"
#include "borrowdialog.h"
#include "ui_searchdialog.h"
#include <QMessageBox>
#include <QDir>
#include<QDebug>
//#include <QStandardItemModel>
//#include <QTableView>

/**
 * @brief SearchDialog::SearchDialog
 * @param parent
 * 搜索界面逻辑
 */
SearchDialog::SearchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchDialog)
{
    ui->setupUi(this);

    ui->book_table->setColumnCount(3);//设置表格的列数目
//    ui->book_table->setRowCount(5);
    //设置表格的列表头
    QStringList header;
    header << "编号" << "标题" << "作者";

    ui->book_table->setHorizontalHeaderLabels(header); //设置行表头
    ui->book_table->horizontalHeader()->setStyleSheet("QHeaderView::section{background:#c4e2fc}");
    ui->book_table->horizontalHeader()->setHighlightSections(false);
    ui->book_table->verticalHeader()->setVisible(false); //隐藏列表头

    ui->book_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->book_table->setColumnWidth(0, 150);
    ui->book_table->setColumnWidth(1, 200);

    QFont font = ui->book_table->horizontalHeader()->font();
    font.setBold(true);
    ui->book_table->horizontalHeader()->setFont(font);

    ui->book_table->horizontalHeader()->setStretchLastSection(true);
//    ui->book_table->verticalHeader()->setDefaultSectionSize(20);

    ui->book_table->setShowGrid(false);

    ui->book_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->book_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    ui->book_table->setStyleSheet("selection-background-color:lightblue");

    connect(this, &SearchDialog::require_borrow_dialog, this, &SearchDialog::open_borrow_dialog);

}

SearchDialog::~SearchDialog()
{
    delete ui;
}

/**
 * @brief SearchDialog::open_borrow_dialog
 * @param file
 * 借阅图书详细界面
 */
void SearchDialog::open_borrow_dialog(QString file)
{
    BorrowDialog * dlg = new BorrowDialog(file, this);
    dlg->setWindowTitle("详细信息");
    dlg->setWindowModality(Qt::ApplicationModal);
    dlg->show();
}


/**
 * @brief SearchDialog::on_Butto_search_clicked
 * 搜索按钮
 */
void SearchDialog::on_Butto_search_clicked()
{
    QDir dir;
    dir.setCurrent("Books");
    qDebug()<<"当前文件夹为"<<dir.currentPath();

    //获取搜索内容
    QString key = ui->lineEdit->text();
    //通过dir类搜索文件
    QStringList filters;
    filters << "*" + key + "*";
    dir.setFilter(QDir::Files);//文件名字作为筛选
    dir.setNameFilters(filters);//传入文件名


    QFileInfoList booklist = dir.entryInfoList();//获取文件信息列表

    QString file_name;

    int count = booklist.size();
    ui->book_table->setRowCount(count);//根据搜索的结果设置表格的行数

    for (int i = 0; i < count; i++)
    {
        QFileInfo file_info = booklist.at(i);
        file_name = file_info.baseName();//读取文件名

        QStringList name_list = file_name.split(" ");//将文件名分解，读取基本信息
        QString index = name_list.at(0);
        QString title = name_list.at(1);
        QString author = name_list.at(2);

        //设置表格item的内容
        QTableWidgetItem * item_index = new QTableWidgetItem();
        item_index->setText(index);
        item_index->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        QTableWidgetItem * item_title = new QTableWidgetItem();
        item_title->setText(title);
        item_title->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        QTableWidgetItem * item_author = new QTableWidgetItem();
        item_author->setText(author);
        item_author->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        //将item显示出来
        ui->book_table->setItem(i, 0, item_index);
        ui->book_table->setItem(i, 1, item_title);
        ui->book_table->setItem(i, 2, item_author);
    }

}

/**
 * @brief SearchDialog::on_book_table_itemDoubleClicked
 * @param item
 * 双击逻辑，开启借书界面。
 */
void SearchDialog::on_book_table_itemDoubleClicked(QTableWidgetItem *item)
{

    int row = item->row();
    QString file_name = ui->book_table->item(row, 0)->text() + " "
            + ui->book_table->item(row, 1)->text() + " "
            + ui->book_table->item(row, 2)->text();

    emit require_borrow_dialog(file_name);

}
