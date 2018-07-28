#ifndef BORROWDIALOG_H
#define BORROWDIALOG_H

#include <QDialog>
#include <QDate>

namespace Ui {
class BorrowDialog;
}

class BorrowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BorrowDialog(QWidget *parent = 0);
    BorrowDialog(QDate cur_date, QString file, QWidget *parent = 0);
    ~BorrowDialog();

signals:

//    void require_borrowsheet(QString spec_index);

private slots:
    void on_table_condition_cellClicked(int row, int column);

//    void open_borrowsheet(QString spec_index);

private:
    Ui::BorrowDialog *ui;
    QString file_name;
    QDate current_date;
};

#endif // BORROWDIALOG_H
