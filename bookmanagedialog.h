#ifndef BOOKMANAGEDIALOG_H
#define BOOKMANAGEDIALOG_H

#include <QDialog>
#include"header.h"

namespace Ui {
class BookManageDialog;
}

class BookManageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BookManageDialog(QWidget *parent = 0);
    ~BookManageDialog();

    QString number;
    QString title;
    QString author;
    QString apartment;
    QString whocan;
    int amount;
    QString info;
    QList<QFileInfo> *fileinfo;

    void get_bookinfo(QString filename);
    void update_booklist();

private slots:
    //void on_list_book_itemClicked(QListWidgetItem *item);

    void on_list_book_currentRowChanged(int currentRow);

    void on_Button_confirm_clicked();

    void on_pushButton_delete_clicked();

private:
    Ui::BookManageDialog *ui;
};

#endif // BOOKMANAGEDIALOG_H
