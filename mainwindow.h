#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

    void require_newbook_dialog();

    void require_search_dialog();

private slots:

    void open_newbook_dialog();

    void open_search_dialog();

    void on_action_help_triggered();

    void on_action_exit_triggered();

    void on_newbook_button_clicked();

    void on_search_button_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
