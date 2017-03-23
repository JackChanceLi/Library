#ifndef BORROWDIALOG_H
#define BORROWDIALOG_H

#include <QDialog>

namespace Ui {
class BorrowDialog;
}

class BorrowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BorrowDialog(QWidget *parent = 0);
    BorrowDialog(QString file, QWidget *parent = 0);
    ~BorrowDialog();

signals:
    void require_borrowsheet(QString action, QString spec_index);

private slots:
    void on_table_condition_cellClicked(int row, int column);

    void open_borrowsheet(QString action, QString spec_index);

private:
    Ui::BorrowDialog *ui;
    QString file_name;
};

#endif // BORROWDIALOG_H
