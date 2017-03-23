#ifndef BORROWSHEETDIALOG_H
#define BORROWSHEETDIALOG_H

#include <QDialog>

namespace Ui {
class BorrowSheetDialog;
}

class BorrowSheetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BorrowSheetDialog(QWidget *parent = 0);
    BorrowSheetDialog(QString action, QString spec_index, QWidget *parent = 0);
    ~BorrowSheetDialog();

private:
    Ui::BorrowSheetDialog *ui;
};

#endif // BORROWSHEETDIALOG_H
