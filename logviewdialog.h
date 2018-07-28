#ifndef LOGVIEWDIALOG_H
#define LOGVIEWDIALOG_H

#include <QDialog>

namespace Ui {
class LogViewDialog;
}

class LogViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LogViewDialog(QWidget *parent = 0);
    ~LogViewDialog();

private:
    Ui::LogViewDialog *ui;
};

#endif // LOGVIEWDIALOG_H
