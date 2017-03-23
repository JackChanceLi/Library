#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QTableWidgetItem>

namespace Ui {
class SearchDialog;
}

class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(QWidget *parent = 0);
    ~SearchDialog();

signals:
    void require_borrow_dialog(QString);

private slots:

    void open_borrow_dialog(QString file);

    void on_Butto_search_clicked();

    void on_book_table_itemDoubleClicked(QTableWidgetItem *item);

private:
    Ui::SearchDialog *ui;
};

#endif // SEARCHDIALOG_H
