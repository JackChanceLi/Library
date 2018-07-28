#include "header.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logindialog.h"
#include "borrowdialog.h"
#include "newbookdialog.h"
#include "bookmanagedialog.h"
#include"dialogfine.h"
#include <QDesktopWidget>
//外部使用的变量
extern User current_user;
QString temp_picsrc;
extern QDate last_date;
extern QString manager_code;
extern QString logfile_name;
extern QString theme;
extern int time_length;
extern int seqinday;

/**
 * @brief 构造函数
 * @param date 当前日期
 */
MainWindow::MainWindow(QDate date, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    current_date = date;
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);

    ui->stackedWidget_function->setCurrentIndex(0);//默认现实借阅信息模块
    ui->tab_condition->setCurrentIndex(0);
    ui->label_date->setText("当前系统时间为："+ QString::number(last_date.daysInYear(),10) +"年 "+QString::number(last_date.month(),10) + "月 "+QString::number(last_date.day(),10)+"日");

    QDesktopWidget* desktop = QApplication::desktop(); //
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

    /**
      *顶部信息展示栏
      */
    if (current_user.get_picsrc() != "")
    {
        QPixmap pix(current_user.get_picsrc());
        ui->label_pic->setPixmap(pix);
        ui->label_pic->setScaledContents(true);
        ui->label_pic->show();
    }
    ui->label_user->setText(current_user.get_username());
    ui->label_credit->setText(QString::number(current_user.get_credit()));
    ui->label_gender->setText(current_user.gender);
    ui->label_number->setText(current_user.number);
    ui->label_identify->setText(current_user.get_identity());
    ui->label_class->setText(current_user.apartment);
    //更改对应的标签
    if (current_user.get_identity() != "学生"){
        ui->label_apartment_head->setText("部门");
        ui->label_num_head->setText("工号");
    }

    //功能按钮部分
    if (current_user.get_identity() == "学生")
    {
        ui->Button_bookmanage->setDisabled(true);
        ui->Button_newbook->setDisabled(true);//管理员功能不可用
    }

    /***************借阅图书信息展示模块***************/
    ui->borrowed_list->clear();
    ui->borrowed_list->setColumnCount(4);
    QStringList borrowlist_header;
    borrowlist_header << "编号" << "标题" << "应还日期" << "操作";
    ui->borrowed_list->setHorizontalHeaderLabels(borrowlist_header);//设置表头
    ui->borrowed_list->horizontalHeader()->setStyleSheet("QHeaderView::section{background:#c4e2fc}");
    ui->borrowed_list->horizontalHeader()->setHighlightSections(false);
    ui->borrowed_list->verticalHeader()->setVisible(false);
    //设置每个列的参数
    ui->borrowed_list->setColumnWidth(0, 130);
    ui->borrowed_list->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    ui->borrowed_list->setColumnWidth(1, 160);
    ui->borrowed_list->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    ui->borrowed_list->setColumnWidth(2, 140);
    ui->borrowed_list->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    ui->borrowed_list->setColumnWidth(3, 130);
    ui->borrowed_list->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Interactive);
    ui->borrowed_list->horizontalHeader()->setStretchLastSection(true);
    ui->borrowed_list->setSelectionBehavior(QAbstractItemView::SelectRows);
    //填充借阅数据
    QList<BorrowRecord> borrow_record_current = current_user.getBorrowRecord();
    int borrow_row_count = borrow_record_current.size();
    if (borrow_row_count != 0)
    {
        ui->borrowed_list->setRowCount(borrow_row_count);
        for (int i = 0; i < borrow_row_count; i++)
        {
            //col 0 : specific index
            QTableWidgetItem * item_spec_index = new QTableWidgetItem();
            item_spec_index->setText(borrow_record_current[i].get_spec_index());
            item_spec_index->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
            ui->borrowed_list->setItem(i, 0, item_spec_index);

            //col 1 : title
            QTableWidgetItem * item_title = new QTableWidgetItem();
            QString title = borrow_record_current[i].get_bookfile_name().split("#")[1];
            item_title->setText(title);
            item_title->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
            ui->borrowed_list->setItem(i, 1, item_title);

            //col 2 : return date
            QDate return_date = QDate::fromString(borrow_record_current[i].get_return_date(), "yyyy MM dd");
            qDebug()<<return_date;
            QString return_date_text = return_date.toString("yyyy/MM/dd");
            QTableWidgetItem * item_return_date = new QTableWidgetItem();
            item_return_date->setText(return_date_text);
            item_return_date->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
            ui->borrowed_list->setItem(i, 2, item_return_date);

            //col 3 : operations
            QTableWidgetItem * item_operation = new QTableWidgetItem();
            item_operation->setText("还书");
            QFont font = item_operation->font();
            font.setUnderline(true);
            item_operation->setFont(font);
            item_operation->setTextColor(Qt::blue);
            item_operation->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
            ui->borrowed_list->setItem(i, 3, item_operation);
        }
    }

    /*************图书预定管理模块*************/
    ui->ordered_list->clear();
    ui->ordered_list->setColumnCount(4);
    QStringList orderlist_header;
    orderlist_header << "编号" << "标题" << "可借时间" << "操作";
    ui->ordered_list->setHorizontalHeaderLabels(orderlist_header);
    ui->ordered_list->horizontalHeader()->setStyleSheet("QHeaderView::section{background:#c4e2fc}");
    ui->ordered_list->horizontalHeader()->setHighlightSections(false);
    ui->ordered_list->verticalHeader()->setVisible(false);
    //列设置
    ui->ordered_list->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    ui->ordered_list->setColumnWidth(0, 161);
    ui->ordered_list->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    ui->ordered_list->setColumnWidth(1, 161);
    ui->ordered_list->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    ui->ordered_list->setColumnWidth(2, 161);
    ui->ordered_list->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Interactive);
    ui->ordered_list->setColumnWidth(3, 161);
    ui->ordered_list->horizontalHeader()->setStretchLastSection(true);
    ui->ordered_list->setSelectionBehavior(QAbstractItemView::SelectRows);
    //载入预定数据
    QList<OrderRecord> order_record_current = current_user.getOrderRecord();
    int order_row_count = order_record_current.size();
    if (order_row_count != 0)
    {
        ui->ordered_list->setRowCount(order_row_count);
        for (int i = 0; i < order_row_count; ++i)
        {
            //col 0 : specific index
            QString spec_index;
            QTableWidgetItem * item_spec_index = new QTableWidgetItem();
            spec_index = order_record_current[i].get_spec_index();
            item_spec_index->setText(spec_index);
            item_spec_index->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
            ui->ordered_list->setItem(i, 0, item_spec_index);

            //col 1 : title
            QTableWidgetItem * item_title = new QTableWidgetItem();
            QString title = order_record_current[i].get_bookfile_name().split("#")[1];
            item_title->setText(title);
            item_title->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
            ui->ordered_list->setItem(i, 1, item_title);

            //col 2 : available date & col 3 : operations
            QDate available_date = QDate::fromString(order_record_current[i].get_available_date(), "yyyy MM dd");
            if (current_date > available_date.addDays(2))
            {
                QMessageBox::warning(this, "请注意", "很抱歉，您预约的《"
                                     + title + "》已经超过了预约预留期限，已自动入库并从您的预约列表中移除。若您还想借阅本书，请到借书/预约界面查看。");
                //change the book's record
                OrderRecord cancel_record = order_record_current[i];
                QString book_file = cancel_record.get_bookfile_name();
                QFile f_book("./Books/" + book_file);
                QByteArray file_data;
                f_book.open(QFile::ReadOnly);
                file_data = f_book.readAll();
                f_book.close();

                QJsonDocument book_doc(QJsonDocument::fromJson(file_data));
                QJsonObject book_obj = book_doc.object();
                QJsonObject book_record = book_obj["record"].toObject();
                QJsonObject the_book = book_record[spec_index].toObject();

                if (the_book["condition"] == 3)
                    the_book["condition"] = 0;
                the_book["order_user"] = "";

                book_record.remove(spec_index);
                book_record.insert(spec_index, the_book);
                book_obj.remove("record");
                book_obj.insert("record", book_record);

                QFile f_book_("./Books/" + book_file);
                f_book_.open(QFile::WriteOnly | QFile::Truncate);
                f_book_.write(QJsonDocument(book_obj).toJson());
                f_book_.close();

                //change the userfile's record
                QFile f_user("user.json");
                QByteArray user_data;
                f_user.open(QFile::ReadOnly);
                user_data = f_user.readAll();
                f_user.close();

                QJsonDocument user_doc(QJsonDocument::fromJson(user_data));
                QJsonObject user_obj = user_doc.object();
                QJsonArray userlist = user_obj["userlist"].toArray();
                for (int i = 0, total = userlist.size(); i < total; ++i)
                {
                    QJsonObject AUser = userlist[i].toObject();
                    if (current_user.get_account() == AUser["account"].toString())
                    {
                        QJsonObject order_records = AUser["order_records"].toObject();
                        order_records.remove(spec_index);
                        AUser.remove("order_records");
                        AUser.insert("order_records", order_records);
                        userlist.removeAt(i);
                        userlist.append(AUser);
                        user_obj.remove("userlist");
                        user_obj.insert("userlist", userlist);
                        break;
                    }
                }
                QFile f_user_("user.json");
                f_user_.open(QFile::WriteOnly | QFile::Truncate);
                f_user_.write(QJsonDocument(user_obj).toJson());
                f_user_.close();


                order_record_current.removeAt(i);
                current_user.updateOrderRecord(order_record_current);

            }
            else if (order_record_current[i].get_condition() == 3 && available_date <= current_date && current_date <= available_date.addDays(2))
            {
                QMessageBox::information(this, "请注意", "您有一本预约的书：《" + title + "》现在可以借阅，请及时借阅。系统将为您预留2天，超过预留期限本书将自动入库。", QMessageBox::Yes);
                QString available_date_text = available_date.toString("yyyy/MM/dd");
                QTableWidgetItem * item_available_date = new QTableWidgetItem();
                item_available_date->setText(available_date_text);
                item_available_date->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
                ui->ordered_list->setItem(i, 2, item_available_date);

                //col 3 : operation
                QTableWidgetItem * item_operation = new QTableWidgetItem();
                item_operation->setText("借阅/取消预约");
                QFont font = item_operation->font();
                font.setUnderline(true);
                item_operation->setFont(font);
                item_operation->setTextColor(Qt::blue);
                item_operation->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
                ui->ordered_list->setItem(i, 3, item_operation);
            }
            else
            {
                QString available_date_text = available_date.toString("yyyy/MM/dd");
                QTableWidgetItem * item_available_date = new QTableWidgetItem();
                item_available_date->setText(available_date_text);
                item_available_date->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
                ui->ordered_list->setItem(i, 2, item_available_date);

                //col 3 : operation
                QTableWidgetItem * item_operation = new QTableWidgetItem();
                item_operation->setText("取消预约");
                QFont font = item_operation->font();
                font.setUnderline(true);
                item_operation->setFont(font);
                item_operation->setTextColor(Qt::blue);
                item_operation->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
                ui->ordered_list->setItem(i, 3, item_operation);
            }
        }
    }

    connect(this, &MainWindow::require_newbook_dialog, this, &MainWindow::open_newbook_dialog);
    connect(this, &MainWindow::require_bookmanage_dialog, this, &MainWindow::open_bookmanage_dialog);
    connect(this, &MainWindow::require_update_mainwindow, this, &MainWindow::update_mainwindow);


}


MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief 菜单功能
 */
//帮助功能
void MainWindow::on_action_help_triggered()
{
    QString text = "你看到了模拟图书馆的借还/管理系统";             //待补充
    QMessageBox::about(this, "帮助", text);
}

//注销登陆
void MainWindow::on_action_switch_triggered()
{
    if(QMessageBox::warning(this, "警告", "确定注销账户吗？", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes){
        LoginDialog * log =new LoginDialog(current_date);
        log->show();
        Tool::systemlog(logfile_name,"注销登陆",current_user.get_username(),"注销登陆成功！");
        close();
    }
}



/**
 * @brief 侧边栏功能逻辑
 */
/*****************借阅&预约按钮ok************************/
//显示操作界面ok
void MainWindow::on_Button_borrow_manage_clicked()
{
    ui->stackedWidget_function->setCurrentIndex(0);
    update_mainwindow();
}
//点击还书逻辑ok
void MainWindow::on_borrowed_list_itemClicked(QTableWidgetItem *item)
{
    if (item->text() != "还书")
        return;
    int row = item->row();//选择的第几个

    if (QMessageBox::warning(this, "确认", "确定要还书吗？", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {
        QString spec_index = ui->borrowed_list->item(row, 0)->text();//获取书编号
        QList<BorrowRecord> record_list = current_user.getBorrowRecord();

        //更改图书文件中的记录
        QString book_file = record_list[row].get_bookfile_name();
        QFile f_book("./Books/" + book_file);
        QByteArray file_data;
        f_book.open(QFile::ReadOnly);
        file_data = f_book.readAll();
        f_book.close();
        Tool::systemlog(logfile_name,"退还书籍请求",current_user.get_username(),"成功退还书籍---"+book_file);
        QJsonDocument book_doc(QJsonDocument::fromJson(file_data));
        QJsonObject book_obj = book_doc.object();
        QJsonObject book_record = book_obj["record"].toObject();
        QJsonObject the_book = book_record[spec_index].toObject();

        //计算信用度
        if (the_book["condition"] == 1 || the_book["condition"] == 2)
        {
            QString return_time = the_book["return_day"].toString();
            QDate deadline_date = QDate::fromString(return_time, "yyyy MM dd");
            int minus = last_date.daysInYear() - deadline_date.daysInYear();
            if (minus < 0)//过期
            {
                int minus_credit = 0;
                if (minus < -10)
                    minus_credit = 5;
                else if (minus < -5)
                    minus_credit = 3;
                else if (minus < 0)
                    minus_credit = 1;

                current_user.set_credit(current_user.get_credit() - minus_credit);
                QMessageBox::information(this, "逾期", "图书逾期归还，信用值降低" + QString::number(minus_credit) + "点，"
                                     + "您目前还剩" + QString::number(current_user.get_credit()) + "点."
                                     + "请及时还书，以增加信用值。信用值低于50点将导致无法借书！");
                Tool::systemlog(logfile_name,"扣除信用积分","计分系统对用户-》"+current_user.get_username(),"扣除信用分"+QString::number(minus_credit));
            }
            else if (minus >= 0)
            {
                if (current_user.get_credit() < 100)
                    current_user.set_credit(current_user.get_credit() + 1);
            }
        }

        //更改图书状态
        if (the_book["condition"] == 1)//归还无预定
        {
            the_book["condition"] = 0;
            the_book["return_day"] = "";
        }
        if (the_book["condition"] == 2)//归还后有预定
        {
            the_book["condition"] = 3;
            the_book["order_until"] = last_date.addDays(2).toString("yyyy MM dd"); //预定状态维持2天
        }
        the_book["current_owner"] = "";

        //写回Json
        book_record.remove(spec_index);
        book_record.insert(spec_index, the_book);
        book_obj.remove("record");
        book_obj.insert("record", book_record);
        QFile f_book_("./Books/" + book_file);
        f_book_.open(QFile::WriteOnly | QFile::Truncate);
        f_book_.write(QJsonDocument(book_obj).toJson());
        f_book_.close();


        //更改用户记录中的借阅信息
        QFile f_user("user.json");
        QByteArray user_data;
        f_user.open(QFile::ReadOnly);
        user_data = f_user.readAll();
        f_user.close();
        QJsonDocument user_doc(QJsonDocument::fromJson(user_data));
        QJsonObject user_obj = user_doc.object();
        QJsonArray userlist = user_obj["userlist"].toArray();
        for (int i = 0, total = userlist.size(); i < total; ++i)
        {
            QJsonObject AUser = userlist[i].toObject();
            if (current_user.get_account() == AUser["account"].toString())
            {
                AUser["credit"] = current_user.get_credit();
                QJsonObject borrow_records = AUser["borrow_records"].toObject();
                borrow_records.remove(spec_index);
                AUser.remove("borrow_records");
                AUser.insert("borrow_records", borrow_records);
                userlist.removeAt(i);
                userlist.insert(i, AUser);
                user_obj.remove("userlist");
                user_obj.insert("userlist", userlist);
            }
            QJsonObject order_records = AUser["order_records"].toObject();
            if (order_records.contains(spec_index))
            {
                QJsonObject the_order_record = order_records[spec_index].toObject();
                the_order_record["days_left"] = 2;
                the_order_record["condition"] = 3;
                the_order_record["available_date"] = current_date.toString("yyyyMMdd");
                order_records.remove(spec_index);
                order_records.insert(spec_index, the_order_record);
                AUser.remove("order_records");
                AUser.insert("order_records", order_records);
                userlist.removeAt(i);
                userlist.insert(i, AUser);
                user_obj.remove("userlist");
                user_obj.insert("userlist", userlist);
            }
        }
        QFile f_user_("user.json");
        f_user_.open(QFile::WriteOnly | QFile::Truncate);
        f_user_.write(QJsonDocument(user_obj).toJson());
        f_user_.close();

        //更新表格信息
        record_list.removeAt(row);
        current_user.updateBorrowRecord(record_list);
        ui->borrowed_list->removeRow(row);
        QMessageBox::information(this, "通知", "还书成功！");
        Tool::systemlog(logfile_name,"退还书籍",current_user.get_username(),"还书成功！");
    }
}
//点击预约逻辑ok
void MainWindow::on_ordered_list_itemClicked(QTableWidgetItem *item)
{
    if (item->text() != "取消预约" && item->text() != "借阅/取消预约")
        return;
    int row = item->row();

    if (item->text() == "借阅/取消预约")//这本预约的书可以借阅，也可以取消预约
    {
        QPushButton * btn_borrow = new QPushButton("借阅本书");
        QPushButton * btn_cancel = new QPushButton("取消预约");
        QMessageBox * operation_choose = new QMessageBox();
        operation_choose->setWindowTitle("操作选择");
        operation_choose->setText("请选择您要执行的操作");
        operation_choose->addButton(btn_borrow, QMessageBox::AcceptRole);
        operation_choose->addButton(btn_cancel, QMessageBox::RejectRole);
        operation_choose->show();
        int return_code = operation_choose->exec();
        if (return_code == QMessageBox::Accepted) //cancel the order.
        {
            QString book_file;
            QString spec_index = ui->ordered_list->item(row, 0)->text();
            QList<OrderRecord> order_list = current_user.getOrderRecord();

            //change the bookfile record
            book_file = order_list[row].get_bookfile_name();
            QFile f_book("./Books/" + book_file);
            QByteArray file_data;
            f_book.open(QFile::ReadOnly);
            file_data = f_book.readAll();
            f_book.close();
            QJsonDocument book_doc(QJsonDocument::fromJson(file_data));
            QJsonObject book_obj = book_doc.object();
            QJsonObject book_record = book_obj["record"].toObject();
            QJsonObject the_book = book_record[spec_index].toObject();
            if (the_book["condition"] == 2)
                the_book["condition"] = 1;
            if (the_book["condition"] == 3)
            {
                the_book["condition"] = 0;
                the_book["days_left"] = 0;
            }
            the_book["order_user"] = "";
            book_record.remove(spec_index);
            book_record.insert(spec_index, the_book);
            book_obj.remove("record");
            book_obj.insert("record", book_record);
            QFile f_book_("./Books/" + book_file);
            f_book_.open(QFile::WriteOnly | QFile::Truncate);
            f_book_.write(QJsonDocument(book_obj).toJson());
            f_book_.close();

            //change the userfile record.
            QFile f_user("user.json");
            QByteArray user_data;
            f_user.open(QFile::ReadOnly);
            user_data = f_user.readAll();
            f_user.close();
            QJsonDocument user_doc(QJsonDocument::fromJson(user_data));
            QJsonObject user_obj = user_doc.object();
            QJsonArray userlist = user_obj["userlist"].toArray();
            for (int i = 0, total = userlist.size(); i < total; ++i)
            {
                QJsonObject AUser = userlist[i].toObject();
                if (current_user.get_account() == AUser["account"].toString())
                {
                    QJsonObject order_records = AUser["order_records"].toObject();
                    order_records.remove(spec_index);
                    AUser.remove("order_records");
                    AUser.insert("order_records", order_records);
                    userlist.removeAt(i);
                    userlist.append(AUser);
                    user_obj.remove("userlist");
                    user_obj.insert("userlist", userlist);
                    break;
                }
            }
            QFile f_user_("user.json");
            f_user_.open(QFile::WriteOnly | QFile::Truncate);
            f_user_.write(QJsonDocument(user_obj).toJson());
            f_user_.close();

            //change the current user's record.
            order_list.removeAt(row);
            current_user.updateOrderRecord(order_list);
            ui->ordered_list->removeRow(row);
            QMessageBox::information(this, "嗯", "取消预约成功");
            Tool::systemlog(logfile_name,"取消预约",current_user.get_username(),"取消预约书籍---"+book_file);
        }
        else if (return_code == QMessageBox::Rejected) //borrow the available book
        {
            QString book_file;
            QString spec_index = ui->ordered_list->item(row, 0)->text();
            QList<OrderRecord> order_list = current_user.getOrderRecord();

            //change the bookfile record
            book_file = order_list[row].get_bookfile_name();
            QFile f_book("./Books/" + book_file);
            QByteArray file_data;
            f_book.open(QFile::ReadOnly);
            file_data = f_book.readAll();
            f_book.close();
            QJsonDocument book_doc(QJsonDocument::fromJson(file_data));
            QJsonObject book_obj = book_doc.object();
            QJsonObject book_record = book_obj["record"].toObject();
            QJsonObject the_book = book_record[spec_index].toObject();

            if (the_book["condition"] == 3)
            {
                //change the bookfile record.
                the_book["condition"] = 1;
                the_book["days_left"] = 30;
                the_book["current_owner"] = current_user.get_account();
                the_book["order_user"] = "";

                book_record.remove(spec_index);
                book_record.insert(spec_index, the_book);
                book_obj.remove("record");
                book_obj.insert("record", book_record);

                QFile f_book_("./Books/" + book_file);
                f_book_.open(QFile::WriteOnly | QFile::Truncate);
                f_book_.write(QJsonDocument(book_obj).toJson());
                f_book_.close();

                //change the userfile record.
                QFile f_user("user.json");
                QByteArray user_data;
                f_user.open(QFile::ReadOnly);
                user_data = f_user.readAll();
                f_user.close();

                QJsonDocument user_doc(QJsonDocument::fromJson(user_data));
                QJsonObject user_obj = user_doc.object();
                QJsonArray userlist = user_obj["userlist"].toArray();
                for (int i = 0, total = userlist.size(); i < total; ++i)
                {
                    QJsonObject AUser = userlist[i].toObject();
                    if (current_user.get_account() == AUser["account"].toString())
                    {
                        QJsonObject order_records = AUser["order_records"].toObject();
                        order_records.remove(spec_index);
                        QJsonObject borrow_records = AUser["borrow_records"].toObject();
                        QJsonObject ABorrowRecord;
                        ABorrowRecord["spec_index"] = spec_index;
                        ABorrowRecord["days_left"] = 30;
                        ABorrowRecord["borrow_date"] = current_date.toString("yyyyMMdd");
                        ABorrowRecord["return_date"] = current_date.addDays(30).toString("yyyyMMdd");
                        ABorrowRecord["condition"] = 1;
                        ABorrowRecord["bookfile_name"] = book_file;
                        borrow_records.insert(spec_index, ABorrowRecord);
                        AUser.remove("order_records");
                        AUser.insert("order_records", order_records);
                        AUser.remove("borrow_records");
                        AUser.insert("borrow_records", borrow_records);
                        userlist.removeAt(i);
                        userlist.append(AUser);
                        user_obj.remove("userlist");
                        user_obj.insert("userlist", userlist);
                        break;
                    }
                }
                QFile f_user_("user.json");
                f_user_.open(QFile::WriteOnly | QFile::Truncate);
                f_user_.write(QJsonDocument(user_obj).toJson());
                f_user_.close();

                //change the current user's record.
                order_list.removeAt(row);
                current_user.updateOrderRecord(order_list);
                BorrowRecord ABorrrowRecord;
                ABorrrowRecord.set_spec_index(spec_index);
                ABorrrowRecord.set_days_left(30);
                ABorrrowRecord.set_bookfile_name(book_file);
                ABorrrowRecord.set_borrow_date(current_date.toString("yyyyMMdd"));
                ABorrrowRecord.set_return_date(current_date.addDays(30).toString("yyyyMMdd"));
                ABorrrowRecord.set_condition(1);
                current_user.addBorrowRecord(ABorrrowRecord);

                ui->ordered_list->removeRow(row);
                QMessageBox::information(this, "嗯", "借书成功");
                Tool::systemlog(logfile_name,"借书请求",current_user.get_username(),"成功借阅书籍---"+book_file);

            }
        }
     }
     else if (item->text() == "取消预约")
     {
        QString book_file;
        if(QMessageBox::warning(this, "嗯", "是否要取消？",QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes){
            QString spec_index = ui->ordered_list->item(row, 0)->text();
            QList<OrderRecord> order_list = current_user.getOrderRecord();

            //change the bookfile record
            book_file = order_list[row].get_bookfile_name();
            QFile f_book("./Books/" + book_file);
            QByteArray file_data;
            f_book.open(QFile::ReadOnly);
            file_data = f_book.readAll();
            f_book.close();
            QJsonDocument book_doc(QJsonDocument::fromJson(file_data));
            QJsonObject book_obj = book_doc.object();
            QJsonObject book_record = book_obj["record"].toObject();
            QJsonObject the_book = book_record[spec_index].toObject();

            if (the_book["condition"] == 2)
                the_book["condition"] = 1;
            the_book["order_user"] = "";

            book_record.remove(spec_index);
            book_record.insert(spec_index, the_book);
            book_obj.remove("record");
            book_obj.insert("record", book_record);

            QFile f_book_("./Books/" + book_file);
            f_book_.open(QFile::WriteOnly | QFile::Truncate);
            f_book_.write(QJsonDocument(book_obj).toJson());
            f_book_.close();

            //change the userfile record.
            QFile f_user("user.json");
            QByteArray user_data;
            f_user.open(QFile::ReadOnly);
            user_data = f_user.readAll();
            f_user.close();

            QJsonDocument user_doc(QJsonDocument::fromJson(user_data));
            QJsonObject user_obj = user_doc.object();
            QJsonArray userlist = user_obj["userlist"].toArray();
            for (int i = 0, total = userlist.size(); i < total; ++i)
            {
                QJsonObject AUser = userlist[i].toObject();
                if (current_user.get_account() == AUser["account"].toString())
                {
                    QJsonObject order_records = AUser["order_records"].toObject();
                    order_records.remove(spec_index);
                    AUser.remove("order_records");
                    AUser.insert("order_records", order_records);
                    userlist.removeAt(i);
                    userlist.append(AUser);
                    user_obj.remove("userlist");
                    user_obj.insert("userlist", userlist);
                    break;
                }
            }
            QFile f_user_("user.json");
            f_user_.open(QFile::WriteOnly | QFile::Truncate);
            f_user_.write(QJsonDocument(user_obj).toJson());
            f_user_.close();

            //change the current user's record.
            order_list.removeAt(row);
            current_user.updateOrderRecord(order_list);
            ui->ordered_list->removeRow(row);
            QMessageBox::information(this, "嗯", "取消预约成功!");
            Tool::systemlog(logfile_name,"取消预约",current_user.get_username(),"取消预约书籍---"+book_file);
        }
     }
}
//双击查看图书详情ok
void MainWindow::on_book_table_itemDoubleClicked(QTableWidgetItem *item)
{
    int row = item->row();
    QString file_name = ui->book_table->item(row, 0)->text() + "#"
            + ui->book_table->item(row, 1)->text() + "#"
            + ui->book_table->item(row, 2)->text() + "#";

    emit require_borrow_dialog(file_name);
}
//打开借阅窗口ok
void MainWindow::open_borrow_dialog(QString file){
    BorrowDialog * dlg = new BorrowDialog(current_date, file, this);
    Tool::systemlog(logfile_name,"查看图书",current_user.get_username(),"查看图书详情---"+file);
    dlg->setWindowTitle("详细信息");
    dlg->exec();
}


/********************新书入库*********************/
void MainWindow::on_Button_newbook_clicked()
{
    emit require_newbook_dialog();
}
void MainWindow::open_newbook_dialog()
{
    NewBookDialog * dlg = new NewBookDialog(this);
    dlg->setWindowTitle("新书入库");
    dlg->exec();
}



/******************图书管理功能********************/
void MainWindow::on_Button_bookmanage_clicked()
{
    emit require_bookmanage_dialog();
}
void MainWindow::open_bookmanage_dialog()
{
    BookManageDialog * dlg = new BookManageDialog();
    dlg->setWindowTitle("图书管理");
    dlg->exec();
}


/******************刷新主界面信息********************/
void MainWindow::on_Button_flush_clicked()
{
    emit require_update_mainwindow();
}
void MainWindow::update_mainwindow()
{
    //刷新主界面信息
    Tool::systemlog(logfile_name,"刷新信息",current_user.get_username(),"刷新界面信息");
    ui->label_user->setText(current_user.get_username());
    ui->label_credit->setText(QString::number(current_user.get_credit()));
    ui->label_gender->setText(current_user.gender);
    ui->label_number->setText(current_user.number);
    ui->label_identify->setText(current_user.get_identity());
    if (current_user.get_identity() == "学生")
        ui->label_class->setText(current_user.apartment);
    else
        ui->label_class->setDisabled(true);

    if (current_user.get_picsrc() != "")
    {
        QPixmap pix(current_user.get_picsrc());
        ui->label_pic->setPixmap(pix);
        ui->label_pic->setScaledContents(true);
        ui->label_pic->show();
    }
    else
    {
        QPixmap pix("头像.jpg");
        ui->label_pic->setPixmap(pix);
        ui->label_pic->setScaledContents(true);
        ui->label_pic->show();
    }

    ui->borrowed_list->clear();
    ui->borrowed_list->setColumnCount(4);
    QStringList borrowlist_header;
    borrowlist_header << "编号" << "标题" << "应还日期" << "操作";
    ui->borrowed_list->setHorizontalHeaderLabels(borrowlist_header);
    ui->borrowed_list->horizontalHeader()->setStyleSheet("QHeaderView::section{background:#c4e2fc}");
    ui->borrowed_list->horizontalHeader()->setHighlightSections(false);
    ui->borrowed_list->verticalHeader()->setVisible(false);
    ui->borrowed_list->setColumnWidth(0, 161);
    ui->borrowed_list->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    ui->borrowed_list->setColumnWidth(1, 161);
    ui->borrowed_list->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    ui->borrowed_list->setColumnWidth(2, 161);
    ui->borrowed_list->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    ui->borrowed_list->setColumnWidth(3, 161);
    ui->borrowed_list->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Interactive);
    ui->borrowed_list->horizontalHeader()->setStretchLastSection(true);
    ui->borrowed_list->setSelectionBehavior(QAbstractItemView::SelectRows);

    QList<BorrowRecord> borrow_record_current = current_user.getBorrowRecord();
    int borrow_row_count = borrow_record_current.size();
    if (borrow_row_count != 0)
    {
        ui->borrowed_list->setRowCount(borrow_row_count);
        for (int i = 0; i < borrow_row_count; ++i)
        {
            QTableWidgetItem * item_spec_index = new QTableWidgetItem();
            item_spec_index->setText(borrow_record_current[i].get_spec_index());
            item_spec_index->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
            ui->borrowed_list->setItem(i, 0, item_spec_index);

            QTableWidgetItem * item_title = new QTableWidgetItem();
            QString title = borrow_record_current[i].get_bookfile_name().split("#")[1];
            item_title->setText(title);
            item_title->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
            ui->borrowed_list->setItem(i, 1, item_title);

            QDate return_date = QDate::fromString(borrow_record_current[i].get_return_date(), "yyyy MM dd");
            QString return_date_text = return_date.toString("yyyy/MM/dd");
            QTableWidgetItem * item_return_date = new QTableWidgetItem();
            item_return_date->setText(return_date_text);
            item_return_date->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
            ui->borrowed_list->setItem(i, 2, item_return_date);

            QTableWidgetItem * item_operation = new QTableWidgetItem();
            item_operation->setText("还书");
            QFont font = item_operation->font();
            font.setUnderline(true);
            item_operation->setFont(font);
            item_operation->setTextColor(Qt::blue);
            item_operation->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
            ui->borrowed_list->setItem(i, 3, item_operation);
        }
    }

    //预定图书界面刷新
    ui->ordered_list->clear();
    ui->ordered_list->setColumnCount(4);
    QStringList orderlist_header;
    orderlist_header << "编号" << "标题" << "可借时间" << "操作";
    ui->ordered_list->setHorizontalHeaderLabels(orderlist_header);
    ui->ordered_list->horizontalHeader()->setStyleSheet("QHeaderView::section{background:#c4e2fc}");
    ui->ordered_list->horizontalHeader()->setHighlightSections(false);
    ui->ordered_list->verticalHeader()->setVisible(false);

    ui->ordered_list->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    ui->ordered_list->setColumnWidth(0, 161);
    ui->ordered_list->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    ui->ordered_list->setColumnWidth(1, 161);
    ui->ordered_list->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    ui->ordered_list->setColumnWidth(2, 161);
    ui->ordered_list->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Interactive);
    ui->ordered_list->setColumnWidth(3, 161);
    ui->ordered_list->horizontalHeader()->setStretchLastSection(true);
    ui->ordered_list->setSelectionBehavior(QAbstractItemView::SelectRows);

    QList<OrderRecord> order_record_current = current_user.getOrderRecord();
    int order_row_count = order_record_current.size();
    if (order_row_count != 0)
    {
        ui->ordered_list->setRowCount(order_row_count);
        for (int i = 0; i < order_row_count; i++)
        {
            QTableWidgetItem * item_spec_index = new QTableWidgetItem();
            item_spec_index->setText(order_record_current[i].get_spec_index());
            item_spec_index->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
            ui->ordered_list->setItem(i, 0, item_spec_index);

            QTableWidgetItem * item_title = new QTableWidgetItem();
            QString title = order_record_current[i].get_bookfile_name().split("#")[1];
            item_title->setText(title);
            item_title->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
            ui->ordered_list->setItem(i, 1, item_title);

            QDate available_date = QDate::fromString(order_record_current[i].get_available_date(), "yyyy MM dd");
            QString available_date_text = available_date.toString("yyyy/MM/dd");
            QTableWidgetItem * item_available_date = new QTableWidgetItem();
            item_available_date->setText(available_date_text);
            item_available_date->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
            ui->ordered_list->setItem(i, 2, item_available_date);

            QTableWidgetItem * item_operation = new QTableWidgetItem();
            item_operation->setText("取消预约");
            QFont font = item_operation->font();
            font.setUnderline(true);
            item_operation->setFont(font);
            item_operation->setTextColor(Qt::blue);
            item_operation->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
            ui->ordered_list->setItem(i, 3, item_operation);
        }
    }
    //QMessageBox::information(this,"通知","刷新完成！","我知道了！");
}

/******************图书检索************************/
void MainWindow::on_Button_searchbook_clicked()
{
    ui->stackedWidget_function->setCurrentIndex(1);
    ui->splitter_result->setVisible(true);
    //将推荐图书载入里边
    ui->lineEdit->clear();

    //表格标题设置
    ui->book_table->setColumnCount(3);
    QStringList header;
    header << "编号" << "标题" << "作者";
    ui->book_table->setHorizontalHeaderLabels(header); //设置行表头
    ui->book_table->horizontalHeader()->setStyleSheet("QHeaderView::section{background:#c4e2fc}");
    ui->book_table->horizontalHeader()->setHighlightSections(false);
    ui->book_table->verticalHeader()->setVisible(false); //隐藏列表头
    ui->book_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->book_table->setColumnWidth(0, 100);
    ui->book_table->setColumnWidth(1, 300);
    ui->book_table->setColumnWidth(2, 240);

    QFont font = ui->book_table->horizontalHeader()->font();
    font.setBold(true);
    ui->book_table->horizontalHeader()->setFont(font);
    ui->book_table->horizontalHeader()->setStretchLastSection(true);

    ui->book_table->setShowGrid(false);
    ui->book_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->book_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
void MainWindow::on_Button_search_clicked()
{
    QDir dir;
    QDir abs_dir = dir.absolutePath();
    if (abs_dir.dirName() != "Books" && !abs_dir.exists("Books"))
    {
        QMessageBox::warning(this, "en", "请先入库一本书");
        close();
        return;
    }

    dir.cd("Books");

    QString key = ui->lineEdit->text();
    Tool::systemlog(logfile_name,"搜索请求",current_user.get_username(),"基于关键字“"+key+"”进行图书搜索！");
    QStringList filters;
    filters << "*" + key + "*";

    dir.setFilter(QDir::Files);
    dir.setNameFilters(filters);

    QFileInfoList booklist = dir.entryInfoList();
    QString file_name;



    //载入表格内容
    int count = booklist.size();
    ui->book_table->setRowCount(count);

    for (int i = 0; i < count; i++)
    {
        QFileInfo file_info = booklist.at(i);
        file_name = file_info.fileName();

        QStringList name_list = file_name.split("#");
        QString index = name_list.at(0);
        QString title = name_list.at(1);
        QString author = name_list.at(2);

        QTableWidgetItem * item_index = new QTableWidgetItem();
        item_index->setText(index);
        item_index->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        QTableWidgetItem * item_title = new QTableWidgetItem();
        item_title->setText(title);
        item_title->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        QTableWidgetItem * item_author = new QTableWidgetItem();
        item_author->setText(author);
        item_author->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        ui->book_table->setItem(i, 0, item_index);
        ui->book_table->setItem(i, 1, item_title);
        ui->book_table->setItem(i, 2, item_author);
    }


    ui->label_search_result->setText("共检索到"+ QString::number(count,10) +"本书");
    ui->splitter_result->setVisible(true);

    connect(this, &MainWindow::require_borrow_dialog, this, &MainWindow::open_borrow_dialog);
}


/*********************账户管理***********************/
void MainWindow::on_Button_profile_manage_clicked()
{
    ui->stackedWidget_function->setCurrentIndex(2);
    //默认不允许更改
    ui->func_lineEdit_name->setEnabled(false);
    ui->func_comboBox_gender->setEnabled(false);
    ui->func_lineEdit_number->setEnabled(false);
    ui->func_lineEdit_class->setEnabled(false);
    ui->func_lineEdit_account->setEnabled(false);
    ui->func_lineEdit_prekey->setEnabled(false);
    ui->func_comboBox_college->setEnabled(false);
    ui->func_comboBox_regyear->setEnabled(false);
    ui->func_lineEdit_newkey->setEnabled(false);
    //载入用户信息
    ui->func_lineEdit_name->setText(current_user.get_username());
    ui->func_comboBox_gender->setCurrentText(current_user.gender);
    ui->func_lineEdit_number->setText(current_user.number);
    ui->func_lineEdit_class->setText(current_user.apartment);
    ui->func_lineEdit_account->setText(current_user.get_account());
    ui->func_lineEdit_prekey->setText(current_user.get_password());
    ui->func_comboBox_college->setCurrentText(current_user.academy);
    ui->func_lcdNumber_credit->setDigitCount(10);
    ui->func_lcdNumber_credit->display(current_user.get_credit());
    ui->func_comboBox_regyear->setCurrentText(current_user.enroll);
    //显示照片
    QPixmap pix(current_user.get_picsrc());
    ui->func_label_pic->setPixmap(pix);
    ui->func_label_pic->setScaledContents(true);
    ui->func_label_pic->show();
    //管理员身份
    if(current_user.get_identity()=="管理员"){
        ui->func_label_number->setText("工号");
        ui->func_label_class->setText("部门");
        ui->func_label_regyear->setText("入职时间");
    }
    update_mainwindow();
}
//更改信息按钮
void MainWindow::on_Button_unlock_change_profile_clicked()
{
    QMessageBox::warning(this, "更改", "现在你可以更改你的信息了！");
    ui->func_lineEdit_name->setEnabled(true);
    ui->func_comboBox_gender->setEnabled(true);
    ui->func_lineEdit_number->setEnabled(true);
    ui->func_lineEdit_class->setEnabled(true);
    ui->func_lineEdit_prekey->setEnabled(true);
    ui->func_comboBox_college->setEnabled(true);
    ui->func_comboBox_regyear->setEnabled(true);
    ui->func_lineEdit_newkey->setEnabled(true);
}
//上传图片按钮
void MainWindow::on_Button_change_pic_clicked()
{
    if((temp_picsrc = QFileDialog::getOpenFileName(this, "上传照片", NULL, "Image(*.jpg *.png)")) != "")
    {
        QPixmap pix(temp_picsrc);
        ui->func_label_pic->setPixmap(pix);
        ui->func_label_pic->setScaledContents(true);
        ui->func_label_pic->show();
    }
}
//提交更改按钮
void MainWindow::on_Button_confirm_change_profile_clicked()
{
     if(QMessageBox::warning(this, "警告", "是否确认提交更改?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
     {
         Tool::systemlog(logfile_name,"更改账户信息",current_user.get_username(),"成功更改账户信息");
         current_user.set_username(ui->func_lineEdit_name->text());
         current_user.gender = ui->func_comboBox_gender->currentText();
         current_user.number = ui->func_lineEdit_number->text();
         current_user.apartment = ui->func_lineEdit_class->text();
         current_user.set_account(ui->func_lineEdit_account->text());
         current_user.academy =  ui->func_comboBox_college->currentText();
         current_user.enroll =  ui->func_comboBox_regyear->currentText();
         if(temp_picsrc != "")
         {
             current_user.set_picsrc(temp_picsrc);
         }
         if(ui->func_lineEdit_newkey->text() != "")
         {
             current_user.set_password(ui->func_lineEdit_newkey->text());
         }

         //保存到user文件
         QFile file("user.json");
         QByteArray val;
         if(file.open(QFile::ReadOnly))
         {
             val = file.readAll();
             file.close();
             qDebug()<<"读取文件成功";
         }

         QJsonDocument file_doc(QJsonDocument::fromJson(val));
         QJsonObject user_data = file_doc.object();

         QJsonArray userlist = user_data["userlist"].toArray();

         for(int i=0;i<userlist.count();i++)
         {
             QJsonObject Auser = userlist[i].toObject();
             qDebug()<<"i = "<<i<<endl;
             if(Auser["account"].toString() == current_user.get_account())
             {
                 //构建用户记录的Json对象
                 Auser["password"] = current_user.get_password();
                 Auser["name"] = current_user.get_username();
                 Auser["gender"] = current_user.gender;
                 Auser["identify"] = current_user.get_identity();
                 Auser["credit"] = current_user.get_credit();
                 Auser["picsrc"] = current_user.get_picsrc();
                 Auser["number"] = current_user.number;
                 Auser["apartment"] = current_user.apartment;
                 Auser["enroll"] = current_user.enroll;
                 Auser["academy"] = current_user.academy;

                 userlist.removeAt(i);
                 userlist.insert(0,Auser);
                 user_data.remove("userlist");
                 user_data.insert("userlist",userlist);

                 //保存到user文件中
                 QJsonDocument savefile(user_data);
                 QFile f_("user.json");
                 f_.open(QIODevice::Truncate | QIODevice::WriteOnly);
                 f_.write(savefile.toJson());
                 f_.close();

                 update_mainwindow();
                 QMessageBox::warning(this, "更改成功", "账户信息更改成功！", "我知道了!");

                 break;
             }
         }
     }
}
//取消更改权限
void MainWindow::on_Button_payfine_2_clicked()
{
    ui->func_lineEdit_name->setEnabled(false);
    ui->func_comboBox_gender->setEnabled(false);
    ui->func_lineEdit_number->setEnabled(false);
    ui->func_lineEdit_class->setEnabled(false);
    ui->func_lineEdit_account->setEnabled(false);
    ui->func_lineEdit_prekey->setEnabled(false);
    ui->func_comboBox_college->setEnabled(false);
    ui->func_comboBox_regyear->setEnabled(false);
    ui->func_lineEdit_newkey->setEnabled(false);
}
//缴纳罚款
void MainWindow::on_Button_payfine_clicked()
{
    if(current_user.get_credit() > 60)
    {
        QMessageBox::warning(this, "罚款缴纳", "您好，您无需缴纳罚款！");
    }
    else{
       QMessageBox::warning(this, "罚款缴纳", "您目前信用值较低，请缴纳罚款以提高信用值");
       Tool::systemlog(logfile_name,"缴纳罚款请求",current_user.get_username(),"请求支付以缴纳罚款");
       DialogFine *dlg_fine = new DialogFine();
       dlg_fine->setWindowTitle("缴纳罚款");
       dlg_fine->exec();

    }

}


/*******************系统设置*************************/
void MainWindow::on_Button_systemsetting_clicked()
{
    ui->stackedWidget_function->setCurrentIndex(3);
    ui->lineEdit_invide_code->setText(manager_code);
    ui->lineEdit_log_file_name->setText(logfile_name);
    bool ok =true;
    ui->comboBox_skin->setCurrentIndex(theme.toInt(&ok,10));
    ui->starttime_edit->setDate(last_date);
    ui->label_timelength->setText(QString::number(time_length,10));

    ui->time_length->setMinimum(0);
    ui->time_length->setMaximum(60);
    ui->time_length->setValue(5);
    //信号和槽 当滑动条的值发生改变时，即产生一个valueChanged(int)信号 设置QLineEdit控件的显示文本
    connect(ui->time_length, SIGNAL(valueChanged(int)), this, SLOT(setLineEditValue()));

    MainWindow::system_info_change(false);
}
//滑块设置时间间隔
void MainWindow::setLineEditValue()
{
  int pos = ui->time_length->value();
  ui->label_timelength->setText("  "+QString::number(pos,10)+"秒");
}


void MainWindow::mousePressEvent(QMouseEvent * event)
{
    if(event->button()==Qt::LeftButton){
        m_Drag = true;
        m_DragPosition = event->globalPos() - this->pos();
        event->accept();
    }

}
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(m_Drag && (event->buttons() && Qt::LeftButton)){
        move(event->globalPos() - m_DragPosition);
        event->accept();
    }
}
void MainWindow::mouseReleaseEvent(QMouseEvent *)
{
    m_Drag = false;
}

//重新启动
void MainWindow::on_Button_payfine_3_clicked()
{
    if(QMessageBox::warning(this,"确定重新启动？","系统将重新启动，确定吗？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes){
        Tool::systemlog(logfile_name,"重新启动",current_user.get_username(),"重新启动模拟系统");
        LoginDialog * log =new LoginDialog(current_date);
        log->show();
        close();
   }
}

/**
 * @brief 打开更改系统配置的权限
 */
void MainWindow::on_Button_unlock_change_profile_2_clicked()
{
    MainWindow::system_info_change(true);
}

void MainWindow::system_info_change(bool ok){

    ui->lineEdit_invide_code->setEnabled(ok);
    ui->time_length->setEnabled(ok);
    ui->comboBox_skin->setEditable(ok);
    ui->lineEdit_log_file_name->setEnabled(ok);
    ui->starttime_edit->setEnabled(ok);
}

/**
 * @brief 提交系统参数的更改
 */
void MainWindow::on_Button_confirm_change_profile_2_clicked()
{
    if(QMessageBox::warning(this,"更改确认？","系统配置将更改确定吗？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes){
        Tool::systemlog(logfile_name,"系统配置更改",current_user.get_username(),"更改系统配置");
        time_length = ui->time_length->value();
        manager_code = ui->lineEdit_invide_code->text();
        logfile_name = ui->lineEdit_log_file_name->text();
        last_date = ui->starttime_edit->date();
        theme = QString::number(ui->comboBox_skin->currentIndex(),10);

        QFile f("config.txt");
        f.open(QFile::ReadWrite | QFile::Text);
        QTextStream infile(&f);

        infile<<QString::number(last_date.year(),10)+" "
             <<last_date.month()<<" "
            <<last_date.day()<<endl;
        infile<<seqinday<<endl;
        infile<<manager_code<<endl;
        infile<<logfile_name<<endl;
        infile<<theme<<endl;
        infile<<QString::number(time_length,10)<<endl;
        f.close();

   }

}
