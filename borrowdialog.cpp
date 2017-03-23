#include "borrowdialog.h"
#include "ui_borrowdialog.h"
#include "borrowsheetdialog.h"
#include <QMessageBox>
#include <QComboBox>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>

BorrowDialog::BorrowDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BorrowDialog)
{
    ui->setupUi(this);
}

BorrowDialog::BorrowDialog(QString file, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BorrowDialog)
{
    ui->setupUi(this);

    file_name = file;
    QStringList filename_list = file_name.split(" ");
    ui->label_title->setText("书名    " + filename_list[1]);
    ui->label_author->setText("作者   " + filename_list[2]);
    ui->label_condition->setText("借阅情况");

    ui->table_condition->setColumnCount(3);
    QStringList header;
    header << "编号" << "借阅情况" << "操作";
    ui->table_condition->setHorizontalHeaderLabels(header);
    ui->table_condition->horizontalHeader()->setStyleSheet("QHeaderView::section{background:#c4e2fc}");
    ui->table_condition->horizontalHeader()->setHighlightSections(false);
    ui->table_condition->verticalHeader()->setVisible(false);

    ui->table_condition->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->table_condition->setColumnWidth(0, 170);
    ui->table_condition->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->table_condition->setColumnWidth(1, 170);
    ui->table_condition->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->table_condition->setColumnWidth(2, 170);
    ui->table_condition->horizontalHeader()->setStretchLastSection(true);

    QFont font = ui->table_condition->horizontalHeader()->font();
    font.setBold(true);
    ui->table_condition->horizontalHeader()->setFont(font);

    ui->table_condition->setShowGrid(false);
//    ui->table_condition->setSelectionBehavior(QAbstractItemView::SelectRows);
//    ui->table_condition->setEditTriggers(QAbstractItemView::NoEditTriggers);


    QDir dir;
//    QMessageBox::warning(this, "en", dir.currentPath());

    QFile f(file_name + ".json");
    if (!f.open(QFile::ReadWrite))
    {
        QMessageBox::warning(this, "en", "wrong! cant open!");
        return;
    }

    QByteArray file_data = f.readAll();
    QJsonDocument file_doc(QJsonDocument::fromJson(file_data));

    QJsonObject file_obj = file_doc.object();
    QJsonObject record_obj = file_obj["record"].toObject();
    ui->label_publisher->setText("出版社   " + file_obj["publisher"].toString());
    ui->label_amount->setText("借阅数量  " + QString::number(file_obj["amount"].toInt()));
    QString intro = file_obj["intro"].toString();
    if (intro == "")
        ui->textBrowser->setText("暂无简介");
    else
        ui->textBrowser->setText(intro);
    QStringList spec_indices = record_obj.keys();

    ui->table_condition->setRowCount(spec_indices.size());

    for (int i = 0; i < spec_indices.size(); i++)
    {
        QTableWidgetItem * spec_index = new QTableWidgetItem();
        spec_index->setText(spec_indices[i]);
        spec_index->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        spec_index->setFlags(Qt::NoItemFlags);
        spec_index->setTextColor(QColor(Qt::black));
        ui->table_condition->setItem(i, 0, spec_index);

        QTableWidgetItem * condition = new QTableWidgetItem();
        QTableWidgetItem * operation = new QTableWidgetItem();
        if (record_obj[spec_indices[i]].toBool() == true)
        {
            condition->setText("在架可借");
            operation->setText("借阅");
            QFont font = operation->font();
            font.setUnderline(true);
            operation->setFont(font);
        }
        else
        {
            condition->setText("借出可预约");
            operation->setText("预约");
            QFont font = operation->font();
            font.setUnderline(true);
            operation->setFont(font);
        }
        condition->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        condition->setFlags(Qt::NoItemFlags);
        condition->setTextColor(QColor(Qt::black));
        ui->table_condition->setItem(i, 1, condition);

        operation->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        operation->setTextColor(QColor(Qt::blue));
        operation->setFlags(Qt::ItemIsSelectable);
        ui->table_condition->setItem(i, 2, operation);

//        ui->table_condition->setItem(i, 2, placeholder);
    }


    f.close();

    connect(this, &BorrowDialog::require_borrowsheet, this, &BorrowDialog::open_borrowsheet);

}

BorrowDialog::~BorrowDialog()
{
    delete ui;
}


void BorrowDialog::on_table_condition_cellClicked(int row, int column)
{
    QString spec_index = ui->table_condition->item(row, 0)->text();
    if (column == 2 && ui->table_condition->item(row, column)->text() == "借阅")
        emit require_borrowsheet("借阅", spec_index);
    else if (column == 2 && ui->table_condition->item(row, column)->text() == "预约")
        emit require_borrowsheet("预约", spec_index);
    else
        return;
}

void BorrowDialog::open_borrowsheet(QString action, QString spec_index)
{
    BorrowSheetDialog * dlg = new BorrowSheetDialog(action, spec_index, this);
    dlg->setWindowTitle(action + "表单");
    dlg->setWindowModality(Qt::ApplicationModal);
    dlg->show();

}
