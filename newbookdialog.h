#ifndef NEWBOOKDIALOG_H
#define NEWBOOKDIALOG_H

#include <QDialog>

namespace Ui {
class NewBookDialog;
}

class NewBookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewBookDialog(QWidget *parent = 0);
    ~NewBookDialog();

private slots:
    void on_Button_save_quit_clicked();

    void on_Button_save_next_clicked();

private:
    Ui::NewBookDialog *ui;
};

#endif // NEWBOOKDIALOG_H
