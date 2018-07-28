#include "logviewdialog.h"
#include "ui_logviewdialog.h"

LogViewDialog::LogViewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogViewDialog)
{
    ui->setupUi(this);
}

LogViewDialog::~LogViewDialog()
{
    delete ui;
}
