#ifndef DIALOGFINE_H
#define DIALOGFINE_H

#include <QDialog>

namespace Ui {
class DialogFine;
}

class DialogFine : public QDialog
{
    Q_OBJECT

public:
    explicit DialogFine(QWidget *parent = 0);
    ~DialogFine();

private slots:
    void on_pushButton_payok_clicked();

    void on_pushButton_payquit_clicked();

private:
    Ui::DialogFine *ui;
};

#endif // DIALOGFINE_H
