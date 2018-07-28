#include "header.h"
#include "borrowdialog.h"
#include "ui_borrowdialog.h"

extern User current_user;
extern QString logfile_name;
extern QDate last_date;
BorrowDialog::BorrowDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BorrowDialog)
{
    ui->setupUi(this);
}
/**
 * @brief 图书借阅
 * @param cur_date 借阅时间
 * @param file
 */
BorrowDialog::BorrowDialog(QDate cur_date, QString file, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BorrowDialog)
{
    ui->setupUi(this);

    current_date = cur_date;//当前系统模拟时间
    file_name = file;//图书所对应的文件名
    QStringList filename_list = file_name.split("#");//对文件名进行解析分割
    ui->label_title->setText(filename_list[1]);
    ui->label_author->setText(filename_list[2]);
    ui->label_condition->setText("借阅/预约情况");
    //相关图书的借阅情况
    ui->table_condition->setColumnCount(3);
    QStringList header;
    header << "编号" << "借阅情况" << "操作";
    ui->table_condition->setHorizontalHeaderLabels(header);
    ui->table_condition->horizontalHeader()->setStyleSheet("QHeaderView::section{background:#c4e2fc}");
    ui->table_condition->horizontalHeader()->setHighlightSections(false);
    ui->table_condition->verticalHeader()->setVisible(false);

    ui->table_condition->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->table_condition->setColumnWidth(0, 170);
    ui->table_condition->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->table_condition->setColumnWidth(1, 170);
    ui->table_condition->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->table_condition->setColumnWidth(2, 170);
    ui->table_condition->horizontalHeader()->setStretchLastSection(true);
    QFont font = ui->table_condition->horizontalHeader()->font();
    font.setBold(true);
    ui->table_condition->horizontalHeader()->setFont(font);
    ui->table_condition->setShowGrid(false);//显示边框

    /**
     * 载入相关的所有图书信息
     */
    QFile f("./Books/" + file_name + ".json");
    if (!f.open(QFile::ReadOnly))
    {
        QMessageBox::warning(this, "错误", "打开文件失败！");
        return;
    }
    QByteArray file_data = f.readAll();
    f.close();
    QJsonDocument file_doc(QJsonDocument::fromJson(file_data));
    QJsonObject file_obj = file_doc.object();
    QJsonObject record_obj = file_obj["record"].toObject();

    ui->label_publisher->setText(file_obj["publisher"].toString());
    ui->label_amount->setText(QString::number(file_obj["amount"].toInt()));
    QString intro = file_obj["intro"].toString();
    if (intro == "")
        ui->textBrowser->setText("暂无简介");
    else
        ui->textBrowser->setText(intro);

    QStringList spec_indices = record_obj.keys();//每本书维持一个记录
    ui->table_condition->setRowCount(spec_indices.size());//基于记录数目，设置表格
    //将信息装载到表格
    for (int i = 0; i < spec_indices.size(); i++)
    {
        //书目编号字段
        QTableWidgetItem * spec_index = new QTableWidgetItem();
        spec_index->setText(spec_indices[i]);
        spec_index->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        spec_index->setFlags(Qt::NoItemFlags);
        spec_index->setTextColor(QColor(Qt::black));
        ui->table_condition->setItem(i, 0, spec_index);

        //借阅情况字段
        QTableWidgetItem * condition = new QTableWidgetItem();
        QTableWidgetItem * operation = new QTableWidgetItem();
        if (record_obj[spec_indices[i]].toObject()["condition"].toInt() == 0)//可借阅
        {
            condition->setText("在架可借");
            operation->setText("借阅");
//            QFont font = operation->font();
//            font.setUnderline(true);
//            operation->setFont(font);
        }
        else if (record_obj[spec_indices[i]].toObject()["condition"].toInt() == 1)//可预约
        {
            condition->setText("借出可预约");
            operation->setText("预约");
//            QFont font = operation->font();
//            font.setUnderline(true);
//            operation->setFont(font);
        }
        else if (record_obj[spec_indices[i]].toObject()["condition"].toInt() == 2)
        {
            condition->setText("借出已有预约");
            operation->setText("请耐心等待");
//            QFont font = operation->font();
//            font.setUnderline(true);
//            operation->setFont(font);
        }
        else if (record_obj[spec_indices[i]].toObject()["condition"].toInt() == 3)
        {
            condition->setText("在架有预约");
            operation->setText("请耐心等待");
//            QFont font = operation->font();
//            font.setUnderline(true);
//            operation->setFont(font);
        }
        condition->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        condition->setFlags(Qt::NoItemFlags);
        condition->setTextColor(QColor(Qt::black));
        ui->table_condition->setItem(i, 1, condition);

        operation->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        operation->setTextColor(QColor(Qt::blue));
        operation->setFlags(Qt::ItemIsSelectable);
        ui->table_condition->setItem(i, 2, operation);


    }

}

BorrowDialog::~BorrowDialog()
{
    delete ui;
}

/**
 * @brief 点击操作
 * @param row
 * @param column
 */
void BorrowDialog::on_table_condition_cellClicked(int row, int column)
{
    //获得点击图书的书目编号
    QString spec_index = ui->table_condition->item(row, 0)->text();
    //用书的编号首位判断借阅权限
    if ((spec_index[1] == '1' && current_user.get_identity() == "学生") || (spec_index[1] == '2' && current_user.get_identity() == "教职工"))
    {
        QMessageBox::warning(this, "权限错误", "抱歉，您无权限借阅/预约本书");
        Tool::systemlog(logfile_name,"权限错误",current_user.get_username(),"身份权限无法借阅图书");
        return;
    }
    //判断信用度是否足够
    if (current_user.get_credit() < 50)
    {
        QMessageBox::warning(this, "信用值过低", "您的信用值低于50点，已无法借书/预约，请在个人管理中缴纳罚款。");
        Tool::systemlog(logfile_name,"信用过低",current_user.get_username(),"信用过低，借阅预约处理失败");
        return;
    }

    /**
      *如果用户选择的图书的目的是借阅
      */
    if (column == 2 && ui->table_condition->item(row, column)->text() == "借阅")
    {
        if (QMessageBox::warning(this, "请确认", "借书天数为30天，确定要借书吗？", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
        {
            /*****将借阅记录写到用户记录中******/
            BorrowRecord ARecord;//组建记录
            ARecord.set_condition(1); //0->1
            ARecord.set_borrow_date(current_date.toString("yyyy MM dd"));
            ARecord.set_return_date(current_date.addDays(30).toString("yyyy MM dd"));
            ARecord.set_days_left(30);
            ARecord.set_spec_index(spec_index);
            ARecord.set_bookfile_name(file_name + ".json");
            current_user.addBorrowRecord(ARecord);
            //保存到用户json文件
            QFile f_user("user.json");
            QByteArray user_data;
            f_user.open(QFile::ReadOnly);
            user_data = f_user.readAll();
            f_user.close();

            QJsonDocument file_doc(QJsonDocument::fromJson(user_data));
            QJsonObject file_obj = file_doc.object();
            QJsonArray userlist = file_obj["userlist"].toArray();
            for (int i = 0; i < userlist.size(); i++)
            {
                QJsonObject AUser = userlist[i].toObject();
                if (current_user.get_account() == AUser["account"].toString())
                {
                    QJsonObject borrow_record;
                    borrow_record["borrow_date"] = ARecord.get_borrow_date();
                    borrow_record["days_left"] = ARecord.get_days_left();
                    borrow_record["return_date"] = ARecord.get_return_date();
                    borrow_record["spec_index"] = spec_index;
                    borrow_record["condition"] = 1;
                    borrow_record["bookfile_name"] = ARecord.get_bookfile_name();

                    QJsonObject borrow_records = AUser["borrow_records"].toObject();
                    borrow_records.insert(ARecord.get_spec_index(), borrow_record);
                    AUser.remove("borrow_records");
                    AUser.insert("borrow_records", borrow_records);
                    userlist.removeAt(i);
                    userlist.append(AUser);
                    file_obj.remove("userlist");
                    file_obj.insert("userlist", userlist);
                    break;
                }
            }
            QFile f_user_("user.json");
            f_user_.open(QFile::WriteOnly | QFile::Truncate);
            f_user_.write(QJsonDocument(file_obj).toJson());
            f_user_.close();


            /*将借阅信息再写到图书json文件中*/
            QFile f_book("./Books/" + file_name + ".json");
            f_book.open(QFile::ReadOnly);
            QByteArray book_data = f_book.readAll();
            f_book.close();
            QJsonDocument bookfile_doc(QJsonDocument::fromJson(book_data));
            QJsonObject bookfile_doc_obj = bookfile_doc.object();
            QJsonObject book_record = bookfile_doc_obj["record"].toObject();
            QJsonObject the_book_obj = book_record[spec_index].toObject();
            //组建记录结构
            the_book_obj["condition"] = 1; //1 means the book has been borrowed but not been ordered.
            the_book_obj["current_owner"] = current_user.get_account();
            the_book_obj["return_day"] = current_date.addDays(30).toString("yyyy MM dd");
            the_book_obj["order_until"] = "";

            book_record.remove(spec_index);
            book_record.insert(spec_index, the_book_obj);
            bookfile_doc_obj.remove("record");
            bookfile_doc_obj.insert("record", book_record);

            QFile f_book_("./Books/" + file_name + ".json");
            f_book_.open(QFile::WriteOnly | QFile::Truncate);
            f_book_.write(QJsonDocument(bookfile_doc_obj).toJson());
            f_book_.close();

            //借阅反馈
            QDate return_date = current_date.addDays(30);
            QMessageBox::information(this, "嗯", "借书成功，请在" + return_date.toString("yyyy年MM月dd日") + "之前归还！", QMessageBox::Yes);
            Tool::systemlog(logfile_name,"借书请求",current_user.get_username(),"借阅成功---"+file_name);
            setVisible(false);
            close();
        }
    }
    /**
      *如果选择的是预定图书
      */
    else if (column == 2 && ui->table_condition->item(row, column)->text() == "预约")
    {
        //读取图书文件中的记录
        QFile f_book("./Books/" + file_name + ".json");
        f_book.open(QFile::ReadOnly);
        QByteArray book_data = f_book.readAll();
        f_book.close();
        QJsonDocument bookfile_doc(QJsonDocument::fromJson(book_data));
        QJsonObject bookfile_doc_obj = bookfile_doc.object();
        QJsonObject book_record = bookfile_doc_obj["record"].toObject();
        QJsonObject the_books_obj = book_record[spec_index].toObject();

        //检测用户是否重复
        if (the_books_obj["current_owner"].toString() == current_user.get_account())
        {
            QMessageBox::warning(this, "错误！", "您已借阅本书，不能同时预约！", QMessageBox::Yes);
            Tool::systemlog(logfile_name,"借阅请求",current_user.get_username(),"借阅请求失败，原因：已经借阅的图书不能同时再预约！");
            return;
        }

        QDate wait_to_day =  QDate::fromString(the_books_obj["return_day"].toString(),"yyyy MM dd");
        int remained_days = wait_to_day.daysInYear() - last_date.daysInYear();
        int condition = the_books_obj["condition"].toInt();

        if(remained_days < 0)
            QMessageBox::warning(this,"通知","上一个借阅者逾期未还！需耐心等待！");
        else if (condition == 1 && QMessageBox::warning(this, "请确认", "距本书归还还有"
                                 + QString::number(remained_days)
                                 + "天, 确认要预约吗？", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
        {

            //将预约信息保存到用户列表中
            OrderRecord ARecord;
            ARecord.set_spec_index(spec_index);
            ARecord.set_condition(2); //1->2
            ARecord.set_bookfile_name(file_name + ".json");
            ARecord.set_days_left(remained_days);
            ARecord.set_available_date(current_date.addDays(remained_days).toString("yyyy MM dd"));
            current_user.addOrderRecord(ARecord);
            //user file.
            QFile f_user("user.json");
            f_user.open(QFile::ReadOnly);
            QByteArray user_data = f_user.readAll();
            f_user.close();
            QJsonDocument user_file_doc(QJsonDocument::fromJson(user_data));
            QJsonObject user_file_obj = user_file_doc.object();
            QJsonArray userlist = user_file_obj["userlist"].toArray();
            for (int i = 0; i < userlist.size(); i++)
            {
                QJsonObject AUser = userlist[i].toObject();
                if (current_user.get_account() == AUser["account"].toString())
                {

                    QJsonObject order_record;
                    order_record["spec_index"] = spec_index;
                    order_record["condition"] = 2;
                    order_record["days_left"] = remained_days;
                    order_record["available_date"] = current_date.addDays(remained_days).toString("yyyy MM dd");
                    order_record["bookfile_name"] = ARecord.get_bookfile_name();

                    QJsonObject order_records = AUser["order_records"].toObject();
                    order_records.insert(ARecord.get_spec_index(), order_record);
                    AUser.remove("order_records");
                    AUser.insert("order_records", order_records);
                    userlist.removeAt(i);
                    userlist.append(AUser);
                    user_file_obj.remove("userlist");
                    user_file_obj.insert("userlist", userlist);
                    break;
                }
            }
            QFile f_user_("user.json");
            f_user_.open(QFile::WriteOnly | QFile::Truncate);
            f_user_.write(QJsonDocument(user_file_obj).toJson());
            f_user_.close();


            //修改图书文件中的记录
            the_books_obj["condition"] = 2;  //2 means the book is borrowed and ordered.
            the_books_obj["order_user"] = current_user.get_account();
            book_record.remove(spec_index);
            book_record.insert(spec_index, the_books_obj);
            bookfile_doc_obj.remove("record");
            bookfile_doc_obj.insert("record", book_record);
            QFile f_book_("./Books/" + file_name + ".json");
            f_book_.open(QFile::WriteOnly | QFile::Truncate);
            f_book_.write(QJsonDocument(bookfile_doc_obj).toJson());
            f_book_.close();

            //预约成功反馈
            Tool::systemlog(logfile_name,"预约请求",current_user.get_username(),"成功预约---"+file_name);
            QMessageBox::information(this, "嗯", "预约成功");
            setVisible(false);
        }
    }
    else
        return;
}
