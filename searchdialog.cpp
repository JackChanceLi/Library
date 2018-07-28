#include "header.h"
#include "searchdialog.h"
#include "ui_searchdialog.h"
#include "borrowdialog.h"
#include "mainwindow.h"

SearchDialog::SearchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchDialog)
{
    ui->setupUi(this);

    ui->book_table->setColumnCount(3);
//    ui->book_table->setRowCount(5);
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
 * @brief 打开借阅对话框
 * @param file
 */
void SearchDialog::open_borrow_dialog(QString file)
{
    BorrowDialog * dlg = new BorrowDialog(file, this);
    dlg->setWindowTitle("详细信息");
//    dlg->setWindowModality(Qt::ApplicationModal);
    dlg->exec();
}


/**
 * @brief 借阅对话框中的搜索按钮逻辑
 */
void SearchDialog::on_Butto_search_clicked()
{
    QDir dir;
    QDir abs_dir = dir.absolutePath();
    if (abs_dir.dirName() != "Books" && !abs_dir.exists("Books"))
    {
//        QMessageBox::warning(this, "en", dir.currentPath());
        QMessageBox::warning(this, "en", "请先入库一本书");
        close();
        return;
    }
//    QMessageBox::warning(this, "en", dir.currentPath());

    dir.cd("Books");

    QString key = ui->lineEdit->text();
    QStringList filters;
    filters << "*" + key + "*";

    dir.setFilter(QDir::Files);
    dir.setNameFilters(filters);

    QFileInfoList booklist = dir.entryInfoList();

    QString file_name;

    int count = booklist.size();
    ui->book_table->setRowCount(count);

    for (int i = 0; i < count; i++)
    {
        QFileInfo file_info = booklist.at(i);
        file_name = file_info.fileName();

        QStringList name_list = file_name.split("#");
        QString index = name_list.at(0);
        QString title = name_list.at(1);
        QString author = name_list.at(2);

        QTableWidgetItem * item_index = new QTableWidgetItem();
        item_index->setText(index);
        item_index->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        QTableWidgetItem * item_title = new QTableWidgetItem();
        item_title->setText(title);
        item_title->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        QTableWidgetItem * item_author = new QTableWidgetItem();
        item_author->setText(author);
        item_author->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        ui->book_table->setItem(i, 0, item_index);
        ui->book_table->setItem(i, 1, item_title);
        ui->book_table->setItem(i, 2, item_author);
    }

}

/**
 * @brief 查询结果双击逻辑
 * @param item
 */
void SearchDialog::on_book_table_itemDoubleClicked(QTableWidgetItem *item)
{

    int row = item->row();
    QString file_name = ui->book_table->item(row, 0)->text() + "#"
            + ui->book_table->item(row, 1)->text() + "#"
            + ui->book_table->item(row, 2)->text() + "#";

    emit require_borrow_dialog(file_name);

}
