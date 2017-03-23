#include "borrowsheetdialog.h"
#include "ui_borrowsheetdialog.h"

BorrowSheetDialog::BorrowSheetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BorrowSheetDialog)
{
    ui->setupUi(this);
}

BorrowSheetDialog::BorrowSheetDialog(QString action, QString spec_index, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BorrowSheetDialog)
{
    ui->setupUi(this);
    ui->groupBox->setTitle(action);
}

BorrowSheetDialog::~BorrowSheetDialog()
{
    delete ui;
}
