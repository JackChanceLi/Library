#include "chsdialog.h"
#include "ui_chsdialog.h"
#include <QMessageBox>

/**
 * @brief ChsDialog::ChsDialog
 * @param parent
 * 身份选择界面，
 */
ChsDialog::ChsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChsDialog)
{
    ui->setupUi(this);
}

ChsDialog::~ChsDialog()
{
    delete ui;
}


/**
 * @brief ChsDialog::on_pushButton_reader_clicked
 * click读者按钮，发送对于信号，并关闭对应选择窗口
 */
void ChsDialog::on_pushButton_reader_clicked()
{
    emit reader_return(ui->pushButton_reader->text());
    close();
}

/**
 * @brief ChsDialog::on_pushButton_manager_clicked
 * click管理员按钮，发送对于信号，并关闭对应选择窗口
 */
void ChsDialog::on_pushButton_manager_clicked()
{
    emit manager_return(ui->pushButton_manager->text());
    close();
}
