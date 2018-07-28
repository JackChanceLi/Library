#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "header.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QDate date, QWidget *parent = 0);
    ~MainWindow();
   bool m_Drag;
    QPoint m_DragPosition;

     void system_info_change(bool ok);

signals:

    void require_newbook_dialog();

    void require_bookmanage_dialog();

    void require_update_mainwindow();

    void require_borrow_dialog(QString);

public slots:

    void update_mainwindow();

private slots:

    void open_newbook_dialog();

    void open_bookmanage_dialog();

    void on_action_help_triggered();

    void on_action_switch_triggered();

    void on_Button_newbook_clicked();

    void on_Button_searchbook_clicked();

    void on_borrowed_list_itemClicked(QTableWidgetItem *item);

    void on_ordered_list_itemClicked(QTableWidgetItem *item);

    void on_Button_flush_clicked();

    void on_Button_bookmanage_clicked();

    void on_Button_borrow_manage_clicked();

    void on_Button_profile_manage_clicked();

    void on_Button_change_pic_clicked();

    void on_Button_unlock_change_profile_clicked();

    void on_Button_confirm_change_profile_clicked();

    void on_Button_payfine_clicked();

    void on_Button_search_clicked();

    void on_book_table_itemDoubleClicked(QTableWidgetItem *item);//双击搜索结果发出借阅信号

    void open_borrow_dialog(QString file);//打开借阅界面

    void mousePressEvent(QMouseEvent * event);

    void mouseMoveEvent(QMouseEvent * event);

    void mouseReleaseEvent(QMouseEvent *);

//    void on_pushButton_clicked();

//    void on_pushButton_6_clicked();

    void on_Button_systemsetting_clicked();

    void on_Button_payfine_2_clicked();

    void on_Button_payfine_3_clicked();

    void on_Button_unlock_change_profile_2_clicked();
    void setLineEditValue();


    void on_Button_confirm_change_profile_2_clicked();

private:
    Ui::MainWindow *ui;
    QDate current_date;
};

#endif // MAINWINDOW_H
