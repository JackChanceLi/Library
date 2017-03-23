#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chsdialog.h"
#include "borrowdialog.h"
#include "newbookdialog.h"
#include "searchdialog.h"
#include <QDesktopWidget>
#include<QMessageBox>
#include"logindialog.h"
#include"header.h"

extern UserBasic current_user;

/**
 * @brief MainWindow::MainWindow
 * @param parent
 * 主界面
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    //绑定操作按钮的信号和槽
    connect(this, &MainWindow::require_newbook_dialog, this, &MainWindow::open_newbook_dialog);
    connect(this, &MainWindow::require_search_dialog, this, &MainWindow::open_search_dialog);
    //展示主界面

    ui->setupUi(this);
    ui->current_user->setText("当前用户："+current_user.username);

}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::open_newbook_dialog
 * 打开新书注册对话界面
 */
void MainWindow::open_newbook_dialog()
{
    NewBookDialog * dlg = new NewBookDialog(this);
    dlg->setWindowTitle("新书入库");
    dlg->setWindowModality(Qt::ApplicationModal);
    dlg->show();
}


/**
 * @brief MainWindow::open_search_dialog
 * 打开显示搜索对话界面
 */
void MainWindow::open_search_dialog()
{
    SearchDialog * dlg = new SearchDialog(this);
    dlg->setWindowTitle("图书检索");
    dlg->setWindowModality(Qt::ApplicationModal);
    dlg->show();
}

void MainWindow::on_action_help_triggered()
{
    QString text = "本系统是模拟图书馆的借还/管理系统。请注意如果你的鼠标和键盘保持不动10秒钟，系统时间就会自动向前推进。";             //待补充
    QMessageBox::about(this, "帮助", text);
}

void MainWindow::on_action_exit_triggered()
{
    QMessageBox::warning(this,"警告","确定注销账户吗？",QMessageBox::Yes,QMessageBox::No);


}


/**
 * @brief MainWindow::on_newbook_button_clicked
 * 按钮click发送信号
 */
void MainWindow::on_newbook_button_clicked()
{
    emit require_newbook_dialog();
}

void MainWindow::on_search_button_clicked()
{
    emit require_search_dialog();
}
