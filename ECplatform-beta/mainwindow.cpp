#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "plat_register.h"
#include "plat_login.h"
#include "addlist.h"
#include "shoplist.h"
#include "product_minus.h"
#include "product_discount.h"
#include "mycard.h"
#include "bind_card.h"
#include <QPushButton>
#include <QDialog>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(1350,800);
    this->setWindowTitle("呱呱电商平台");
    state=EMPTY;
    fulfill=0;
    tominus=0;
    discount=1;
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->setTabText(0,"图书");
    ui->tabWidget->setTabText(1,"食品");
    ui->tabWidget->setTabText(2,"服装");
    InitBook();
    InitFood();
    InitClothes();
    changeWindow();

}

void MainWindow::changeWindow()
{
    switch (state) {
    case EMPTY:
        ui->showinfo->setVisible(false);
        ui->shoplist->hide();
        ui->setDiscount->hide();
        ui->discount2->hide();
        ui->login->show();
        ui->registered->show();
        ui->admin_login->show();
        ui->addlist->show();
        ui->LogoutBtn->hide();
        ui->BindBtn->hide();
        break;
    case LOGIN:
        ui->showinfo->setVisible(true);
        ui->shoplist->show();
        ui->setDiscount->hide();
        ui->discount2->hide();
        ui->login->hide();
        ui->registered->hide();
        ui->admin_login->hide();
        ui->addlist->show();
        ui->LogoutBtn->show();
        ui->BindBtn->show();
        break;
    case ADMIN:
        ui->showinfo->setText("当前账户：管理员");
        ui->showinfo->setVisible(true);
        ui->shoplist->hide();
        ui->setDiscount->show();
        ui->discount2->show();
        ui->login->hide();
        ui->registered->hide();
        ui->admin_login->hide();
        ui->addlist->hide();
        ui->LogoutBtn->show();
        ui->BindBtn->hide();
        break;
    default:
        break;
    }
}

void MainWindow::slot_append()
{
    QPushButton *senderObj=qobject_cast<QPushButton*>(sender());
    if(senderObj == nullptr)
    {
        return;
    }
    QModelIndex idx =ui->book->indexAt(QPoint(senderObj->frameGeometry().x(),senderObj->frameGeometry().y()));
    int row=idx.row();
    if(state == EMPTY)
    {
        QMessageBox::information(NULL,"Error","请先登录",QMessageBox::Yes);
    }
    else
    {
        if(Book[row].getRemain() > 0)
        {
            addlist al;
            if(al.exec() == QDialog::Accepted)
            {

            }
        }
        else
        {
            QMessageBox::information(NULL,"Error","该商品已无库存",QMessageBox::Yes);
            return;
        }
    }
}

void MainWindow::discount_receive(double x,QString y)
{
    discount=x;
    std::vector<QString> formname;
    std::vector<QString> changename;
    std::vector<QString> changedesc;
    std::vector<int> changecount;
    QSqlDatabase data,data2;
    data=QSqlDatabase::addDatabase("QSQLITE","product");
    data.setDatabaseName("products.db");
    data2=QSqlDatabase::addDatabase("QSQLITE","list");
    data2.setDatabaseName("lists.db");
    if(y == "图书")
    {
        y="Book";
    }
    else if(y == "食品")
    {
        y="Food";
    }
    else if(y == "服装")
    {
        y="Clothes";
    }
    else
    {
        QMessageBox::information(NULL,"Error","discount error",QMessageBox::Yes);
        return;
    }
    if(!data2.open())
    {
        qDebug() << "Error:Failed to connect lists.db" << data.lastError();
        QMessageBox::information(NULL,"Error","打开商品数据库lists.db失败",QMessageBox::Yes);
        return;
    }
    if(!data.open())
    {
        qDebug() << "Error:Failed to connect products.db" << data.lastError();
        QMessageBox::information(NULL,"Error","打开商品数据库products.db失败",QMessageBox::Yes);
        return;
    }
    else
    {
        QSqlQuery query_product(data);
        QSqlQuery query_list(data2);
        QString update_info="UPDATE \""+y+"\" SET discount=:discount";
        query_product.prepare(update_info);
        query_product.bindValue(":discount",x);
        if(!query_product.exec())
        {
            qDebug()<<query_product.lastError();
            QMessageBox::information(NULL,"Error","更改折扣失败",QMessageBox::Yes);
            return;
        }
        else
        {
            if(y == "Book")
            {
                y="图书";
                QMessageBox::information(NULL,"Success","更改"+y+"类折扣成功",QMessageBox::Yes);
                if(x!=1)
                    ui->book_label->setText("全场"+y+"类享受折扣"+QString::number(x,'f',2));
                if(x==1)
                    ui->book_label->setText("");
                QStringList tables=data2.tables();
                QStringListIterator itr(tables);
                while(itr.hasNext())
                {
                    QString tableName = itr.next().toLocal8Bit();
                    formname.push_back(tableName);
                }
                for(unsigned int j=0;j<formname.size();j++)
                {
                    QString select_list="SELECT name,describe,count FROM \""+formname[j]+"\" WHERE type==\""+y+"\"";
                    query_list.exec(select_list);
                    while(query_list.next())
                    {
                        QString tmp_name=query_list.value(0).toString();
                        QString tmp_desc=query_list.value(1).toString();
                        int tmp_count=query_list.value(2).toInt();
                        changename.push_back(tmp_name);
                        changedesc.push_back(tmp_desc);
                        changecount.push_back(tmp_count);
                    }

                    for(unsigned int k=0;k<changedesc.size();k++)
                    {
                        QString select_info="SELECT price,discount FROM Book WHERE name==\""+changename[k]+"\"AND describe==\""+changedesc[k]+"\"";//改
                        query_product.exec(select_info);
                        query_product.next();
                        int tmp_price=query_product.value(0).toInt();

                        double tmp_discount=query_product.value(1).toDouble();
                        QString update_list="UPDATE \""+formname[j]+"\" SET price=:price,total=:total WHERE type==\""+y\
                                +"\" AND name==\""+changename[k]+"\" AND describe==\""+changedesc[k]+"\"";
                        query_list.prepare(update_list);
                        query_list.bindValue(":price",tmp_price*tmp_discount);
                        query_list.bindValue(":total",changecount[k]*tmp_price*tmp_discount);
                        if(!query_list.exec())
                        {
                            qDebug()<<query_product.lastError();
                            QMessageBox::information(NULL,"Error","更改折扣失败",QMessageBox::Yes);
                            return;
                        }
                    }
                    changename.clear();
                    changedesc.clear();
                    changecount.clear();
                }
            }
            else if(y == "Food")
            {
                y="食品";
                QMessageBox::information(NULL,"Success","更改"+y+"类折扣成功",QMessageBox::Yes);
                if(x!=1)
                    ui->food_label->setText("全场"+y+"类享受折扣"+QString::number(x,'f',2));
                if(x==1)
                    ui->food_label->setText("");
                QStringList tables=data2.tables();
                QStringListIterator itr(tables);
                while(itr.hasNext())
                {
                    QString tableName = itr.next().toLocal8Bit();
                    formname.push_back(tableName);
                }
                for(unsigned int j=0;j<formname.size();j++)
                {
                    QString select_list="SELECT name,describe,count FROM \""+formname[j]+"\" WHERE type==\""+y+"\"";
                    query_list.exec(select_list);
                    while(query_list.next())
                    {
                        QString tmp_name=query_list.value(0).toString();
                        QString tmp_desc=query_list.value(1).toString();
                        int tmp_count=query_list.value(2).toInt();
                        changename.push_back(tmp_name);
                        changedesc.push_back(tmp_desc);
                        changecount.push_back(tmp_count);
                    }

                    for(unsigned int k=0;k<changedesc.size();k++)
                    {
                        QString select_info="SELECT price,discount FROM Food WHERE name==\""+changename[k]+"\"AND describe==\""+changedesc[k]+"\"";//改
                        query_product.exec(select_info);
                        query_product.next();
                        int tmp_price=query_product.value(0).toInt();

                        double tmp_discount=query_product.value(1).toDouble();
                        QString update_list="UPDATE \""+formname[j]+"\" SET price=:price,total=:total WHERE type==\""+y\
                                +"\" AND name==\""+changename[k]+"\" AND describe==\""+changedesc[k]+"\"";
                        query_list.prepare(update_list);
                        query_list.bindValue(":price",tmp_price*tmp_discount);
                        query_list.bindValue(":total",changecount[k]*tmp_price*tmp_discount);
                        if(!query_list.exec())
                        {
                            QMessageBox::information(NULL,"Error","更改折扣失败",QMessageBox::Yes);
                            return;
                        }
                    }
                    changename.clear();
                    changedesc.clear();
                    changecount.clear();
                }
            }
            else if(y == "Clothes")
            {
                y="服装";
                QMessageBox::information(NULL,"Success","更改"+y+"类折扣成功",QMessageBox::Yes);
                if(x!=1)
                    ui->clothes_label->setText("全场"+y+"类享受折扣"+QString::number(x,'f',2));
                if(x==1)
                    ui->clothes_label->setText("");
                QStringList tables=data2.tables();
                QStringListIterator itr(tables);
                while(itr.hasNext())
                {
                    QString tableName = itr.next().toLocal8Bit();
                    formname.push_back(tableName);
                }
                for(unsigned int j=0;j<formname.size();j++)
                {
                    QString select_list="SELECT name,describe,count FROM \""+formname[j]+"\" WHERE type==\""+y+"\"";
                    query_list.exec(select_list);
                    while(query_list.next())
                    {
                        QString tmp_name=query_list.value(0).toString();
                        QString tmp_desc=query_list.value(1).toString();
                        int tmp_count=query_list.value(2).toInt();
                        changename.push_back(tmp_name);
                        changedesc.push_back(tmp_desc);
                        changecount.push_back(tmp_count);
                    }

                    for(unsigned int k=0;k<changedesc.size();k++)
                    {
                        QString select_info="SELECT price,discount FROM Clothes WHERE name==\""+changename[k]+"\"AND describe==\""+changedesc[k]+"\"";//改
                        query_product.exec(select_info);
                        query_product.next();
                        int tmp_price=query_product.value(0).toInt();

                        double tmp_discount=query_product.value(1).toDouble();
                        QString update_list="UPDATE \""+formname[j]+"\" SET price=:price,total=:total WHERE type==\""+y\
                                +"\" AND name==\""+changename[k]+"\" AND describe==\""+changedesc[k]+"\"";
                        query_list.prepare(update_list);
                        query_list.bindValue(":price",tmp_price*tmp_discount);
                        query_list.bindValue(":total",changecount[k]*tmp_price*tmp_discount);
                        if(!query_list.exec())
                        {
                            QMessageBox::information(NULL,"Error","更改折扣失败",QMessageBox::Yes);
                            qDebug()<<query_list.lastError();
                            return;
                        }
                    }
                    changename.clear();
                    changedesc.clear();
                    changecount.clear();
                }
            }
        }
    }
    data.close();
    data2.close();
    InitBook();
    InitFood();
    InitClothes();
}

void MainWindow::tominus_receive(double x,double y)
{
    fulfill=x;
    tominus=y;
    if(tominus!=0)
    {
        ui->minus_label->setText("单笔订单满"+QString::number(fulfill,'f',2)+"减"+QString::number(tominus,'f',2));
    }
    else
    {
        ui->minus_label->setText("");
    }
}

void MainWindow::result_receive(user_list x)
{
    QSqlDatabase data;
    data=QSqlDatabase::addDatabase("QSQLITE");
    data.setDatabaseName("products.db");
    if(!data.open())
    {
        qDebug() << "Error in Check:Failed to connect products" << data.lastError();
        QMessageBox::information(NULL,"Error","无法打开商品数据库文件products.db，请检查",QMessageBox::Yes);
        return;
    }
    QSqlQuery query(data);
    if(x.getType()=="图书")
    {
        for(unsigned int i=0;i<Book.size();i++)
        {
            if(Book[i].getDesc()==x.getDesc())
            {
                QString update_info="UPDATE Book SET remain=:remain WHERE name==\""+x.getName()+"\""+" AND describe==\""+x.getDesc()+"\"";
                query.prepare(update_info);
                int tmp=Book[i].getRemain();
                query.bindValue(":remain",tmp-x.getCount());
                if(!query.exec())
                {
                    qDebug() << "Error in result:Failed to update Book" << data.lastError();
                }
                data.close();
                InitBook();
                return;
            }
        }
    }
    else if(x.getType()=="食品")
    {
        for(unsigned int i=0;i<Food.size();i++)
        {
            if(Food[i].getDesc()==x.getDesc())
            {
                int tmp=Food[i].getRemain();
                QString update_info="UPDATE Food SET remain=:remain WHERE name==\""+x.getName()+"\""+" AND describe==\""+x.getDesc()+"\"";
                query.prepare(update_info);
                query.bindValue(":remain",tmp-x.getCount());
                if(!query.exec())
                {
                    qDebug() << "Error in result:Failed to update Food" << data.lastError();
                }
                data.close();
                InitFood();
                return;
            }
        }
    }
    else if(x.getType()=="服装")
    {
        for(unsigned int i=0;i<Clothes.size();i++)
        {
            if(Clothes[i].getDesc()==x.getDesc())
            {
                int tmp=Clothes[i].getRemain();
                QString update_info="UPDATE Clothes SET remain=:remain WHERE name==\""+x.getName()+"\""+" AND describe==\""+x.getDesc()+"\"";
                query.prepare(update_info);
                query.bindValue(":remain",tmp-x.getCount());
                if(!query.exec())
                {
                    qDebug() << "Error in result:Failed to update Clothes" << data.lastError();
                }
                data.close();
                InitClothes();
                return;
            }
        }
    }
}

void MainWindow::receive_user(User tmp)
{
    login_user=tmp;
    ui->showinfo->setText("当前账户："+login_user.getId());
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_login_clicked()
{
    plat_login pl;
    QObject::connect(&pl,SIGNAL(send_user(User)),this,SLOT(receive_user(User)));
    if(pl.exec() == QDialog::Accepted)
    {
        state=LOGIN;

        changeWindow();
    }
    //pl.exec();
}

void MainWindow::on_registered_clicked()
{
    plat_register pr;
    pr.exec();
}

void MainWindow::on_shoplist_clicked()
{
    shoplist sl;
    connect(&sl,SIGNAL(send_result(user_list)),this,SLOT(result_receive(user_list)));
    sl.setUser(login_user);
    sl.setTominus(fulfill,tominus);
    sl.setDiscnt(discount);
    sl.add();
    sl.Init();
    sl.exec();
}

void MainWindow::on_setDiscount_clicked()
{
    product_discount pd;
    connect(&pd,SIGNAL(send_discount(double,QString)),this,SLOT(discount_receive(double,QString)));
    pd.exec();
}

void MainWindow::on_discount2_clicked()
{
    product_minus pm;
    connect(&pm,SIGNAL(send_tominus(double,double)),this,SLOT(tominus_receive(double,double)));
    pm.exec();
}

void MainWindow::on_addlist_clicked()
{
    if(state == EMPTY)
    {
        QMessageBox::information(NULL,"Error","请先登录",QMessageBox::Yes);
        return;
    }
    int page=ui->tabWidget->currentIndex();
    if(page==0)
    {
        bool focus=ui->book->isItemSelected(ui->book->currentItem());
        int row;
        if(focus)
        {
            row=ui->book->currentItem()->row();
        }
        else
        {
            QMessageBox::information(NULL,"Error","请选择要添加进购物车的商品",QMessageBox::Yes);
            return;
        }
        addlist al;
        al.setUser(login_user);
        al.setBook(Book[row]);
        al.setPage(page);
        if(al.exec() == QDialog::Accepted)
        {
            QMessageBox::information(NULL,"Success","添加成功",QMessageBox::Yes);
        }

    }
    else if(page==1)
    {
        bool focus=ui->food->isItemSelected(ui->food->currentItem());
        int row;
        if(focus)
        {
            row=ui->food->currentItem()->row();
        }
        else
        {
            QMessageBox::information(NULL,"Error","请选择要添加进购物车的商品",QMessageBox::Yes);
            return;
        }
        addlist al;
        al.setUser(login_user);
        al.setFood(Food[row]);
        al.setPage(page);
        if(al.exec() == QDialog::Accepted)
        {
            QMessageBox::information(NULL,"Success","添加成功",QMessageBox::Yes);
        }
    }
    else if(page==2)
    {
        bool focus=ui->clothes->isItemSelected(ui->clothes->currentItem());
        int row;
        if(focus)
        {
            row=ui->clothes->currentItem()->row();
        }
        else
        {
            QMessageBox::information(NULL,"Error","请选择要添加进购物车的商品",QMessageBox::Yes);
            return;
        }
        addlist al;
        al.setUser(login_user);
        al.setClothes(Clothes[row]);
        al.setPage(page);
        if(al.exec() == QDialog::Accepted)
        {
            QMessageBox::information(NULL,"Success","添加成功",QMessageBox::Yes);
        }
    }
}

void MainWindow::on_admin_login_clicked()
{
    state=ADMIN;
    changeWindow();
}

void MainWindow::on_LogoutBtn_clicked()
{
    state=EMPTY;
    User tmp;
    login_user=tmp;
    QMessageBox::information(NULL,"Success","注销成功",QMessageBox::Yes);
    changeWindow();
}

void MainWindow::InitBook()
{
    ui->book->clear();
    Book.clear();
    QSqlDatabase data;
    data=QSqlDatabase::addDatabase("QSQLITE");
    data.setDatabaseName("products.db");
    if(!data.open())
    {
        qDebug() << "Error in Check:Failed to connect products" << data.lastError();
        QMessageBox::information(NULL,"Error","无法打开商品数据库文件，请检查",QMessageBox::Yes);
        return;
    }
    else
    {
        QSqlQuery query(data);
        QString select_info="SELECT * from Book";
        if(!query.exec(select_info))
        {
            qDebug()<<query.lastError();
            QMessageBox::information(NULL,"Error","无法打开图书数据库文件products.db，请检查",QMessageBox::Yes);
            return;
        }
        else
        {
            while(query.next())
            {
                book tmp;
                tmp.setName(query.value(0).toString());
                tmp.setAuthor(query.value(1).toString());
                tmp.setDesc(query.value(2).toString());
                tmp.setPrice(query.value(3).toDouble());
                tmp.setRemain(query.value(4).toInt());
                tmp.setDiscount(query.value(5).toDouble());
                Book.push_back(tmp);
            }
        }
    }
    //初始化表格
    //ui->book->setWindowTitle(QObject::tr("电商平台"));  //表名
    ui->book->setEditTriggers(QAbstractItemView::NoEditTriggers);   //表格禁止编辑
    ui->book->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式
   //设置行列数
    ui->book->setColumnCount(7);
    ui->book->setRowCount(1);
   //设置每列宽
    for(int i=0;i<7;i++)
    {
        ui->book->setColumnWidth(i,109);
    }
    ui->book->setColumnWidth(2,600);
    //设置表头
    QStringList header;
    header.append("书名");
    header.append("作者");
    header.append("简介");
    header.append("剩余量");
    header.append("原价");
    header.append("折扣");
    header.append("现价");
    ui->book->setHorizontalHeaderLabels(header);
    ui->book->setRowCount(1);//表格加一行
    for (int i=0;i<7;i++)
    {
        ui->book->setItem(1,i,new QTableWidgetItem(""));
    }
    //ui->book->QTableView::resizeRowsToContents();

    ui->book->setRowCount(Book.size());
    for(unsigned int i=0;i<Book.size();i++)
    {
        ui->book->setItem(i,0,new QTableWidgetItem(Book[i].getName()));
        ui->book->setItem(i,1,new QTableWidgetItem(Book[i].getAuthor()));
        ui->book->setItem(i,2,new QTableWidgetItem(Book[i].getDesc()));
        ui->book->setItem(i,3,new QTableWidgetItem(QString::number(Book[i].getRemain(),10)));
        ui->book->setItem(i,4,new QTableWidgetItem(QString::number(Book[i].product::getPrice(),'f',2)));
        ui->book->setItem(i,5,new QTableWidgetItem(QString::number(Book[i].getDiscount(),'f',2)));
        ui->book->setItem(i,6,new QTableWidgetItem(QString::number(Book[i].getPrice(),'f',2)));
        //QPushButton *m_button = new QPushButton("添加");
        //connect(m_button,SIGNAL(clicked()),this,SLOT(slot_append()));
        //ui->book->setCellWidget(i,7,m_button);
        ui->book->QTableView::resizeRowsToContents();
    }
    ui->book->QTableView::resizeRowsToContents();
    ui->book->update();
    data.close();
}

void MainWindow::InitFood()
{
    ui->food->clear();
    Food.clear();
    QSqlDatabase data;
    data=QSqlDatabase::addDatabase("QSQLITE");
    data.setDatabaseName("products.db");
    if(!data.open())
    {
        qDebug() << "Error in Check:Failed to connect products" << data.lastError();
        QMessageBox::information(NULL,"Error","无法打开商品数据库文件",QMessageBox::Yes);
        return;
    }
    else
    {
        QSqlQuery query(data);
        QString select_info="SELECT * from Food";
        if(!query.exec(select_info))
        {
            qDebug()<<query.lastError();
            QMessageBox::information(NULL,"Error","无法打开食品数据库文件",QMessageBox::Yes);
            return;
        }
        else
        {
            while(query.next())
            {
                food tmp;
                tmp.setName(query.value(0).toString());
                tmp.setMaintain(query.value(1).toString());
                tmp.setDesc(query.value(2).toString());
                tmp.setPrice(query.value(3).toDouble());
                tmp.setRemain(query.value(4).toInt());
                tmp.setDiscount(query.value(5).toDouble());
                Food.push_back(tmp);
            }
        }
    }
    //初始化表格
    //ui->food->setWindowTitle(QObject::tr("电商平台"));  //表名
    ui->food->setEditTriggers(QAbstractItemView::NoEditTriggers);   //表格禁止编辑
    ui->food->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式
   //设置行列数
    ui->food->setColumnCount(7);
    ui->food->setRowCount(1);
   //设置每列宽
    for (int i=0;i<7;i++)
    {
        ui->food->setColumnWidth(i,109);
    }
    ui->food->setColumnWidth(2,600);
    //设置表头
    QStringList header;
    header.append("商品名");
    header.append("主要成分");
    header.append("简介");
    header.append("剩余量");
    header.append("原价");
    header.append("折扣");
    header.append("现价");
    ui->food->setHorizontalHeaderLabels(header);
    ui->food->setRowCount(1);//表格加一行
    for (int i=0;i<7;i++)
    {
        ui->food->setItem(1,i,new QTableWidgetItem(""));
    }
    //ui->food->QTableView::resizeRowsToContents();
    ui->food->setRowCount(Food.size());
    for(unsigned int i=0;i<Food.size();i++)
    {
        ui->food->setItem(i,0,new QTableWidgetItem(Food[i].getName()));
        ui->food->setItem(i,1,new QTableWidgetItem(Food[i].getMaintain()));
        ui->food->setItem(i,2,new QTableWidgetItem(Food[i].getDesc()));
        ui->food->setItem(i,3,new QTableWidgetItem(QString::number(Food[i].getRemain(),10)));
        ui->food->setItem(i,4,new QTableWidgetItem(QString::number(Food[i].product::getPrice(),'f',2)));
        ui->food->setItem(i,5,new QTableWidgetItem(QString::number(Food[i].getDiscount(),'f',2)));
        ui->food->setItem(i,6,new QTableWidgetItem(QString::number(Food[i].getPrice(),'f',2)));
        //QPushButton *m_button = new QPushButton("添加");
        //connect(m_button,SIGNAL(clicked()),this,SLOT(slot_append()));
        //ui->food->setCellWidget(i,7,m_button);
        ui->food->QTableView::resizeRowsToContents();
    }
    ui->food->QTableView::resizeRowsToContents();
    ui->food->update();
    data.close();
}

void MainWindow::InitClothes()
{
    ui->clothes->clear();
    Clothes.clear();
    QSqlDatabase data;
    data=QSqlDatabase::addDatabase("QSQLITE");
    data.setDatabaseName("products.db");
    if(!data.open())
    {
        qDebug() << "Error in Check:Failed to connect products" << data.lastError();
        QMessageBox::information(NULL,"Error","无法打开商品数据库文件",QMessageBox::Yes);
        return;
    }
    else
    {
        QSqlQuery query(data);
        QString select_info="SELECT * from Clothes";
        if(!query.exec(select_info))
        {
            qDebug()<<query.lastError();
            QMessageBox::information(NULL,"Error","无法打开服装数据库文件",QMessageBox::Yes);
            return;
        }
        else
        {
            while(query.next())
            {
                clothes tmp;
                tmp.setName(query.value(0).toString());
                tmp.setAttr(query.value(1).toString());
                tmp.setDesc(query.value(2).toString());
                tmp.setPrice(query.value(3).toDouble());
                tmp.setRemain(query.value(4).toInt());
                tmp.setDiscount(query.value(5).toDouble());
                Clothes.push_back(tmp);
            }
        }
    }
    //初始化表格
    //ui->clothes->setWindowTitle(QObject::tr("电商平台"));  //表名
    ui->clothes->setEditTriggers(QAbstractItemView::NoEditTriggers);   //表格禁止编辑
    ui->clothes->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式
   //设置行列数
    ui->clothes->setColumnCount(7);
    ui->clothes->setRowCount(1);
   //设置每列宽
    for (int i=0;i<7;i++)
    {
        ui->clothes->setColumnWidth(i,109);
    }
    ui->clothes->setColumnWidth(2,600);
    //设置表头
    QStringList header;
    header.append("商品名");
    header.append("属性增幅");
    header.append("简介");
    header.append("剩余量");
    header.append("原价");
    header.append("折扣");
    header.append("现价");
    ui->clothes->setHorizontalHeaderLabels(header);
    ui->clothes->setRowCount(1);//表格加一行
    for (int i=0;i<7;i++)
    {
        ui->clothes->setItem(1,i,new QTableWidgetItem(""));
    }
    //ui->clothes->QTableView::resizeRowsToContents();
    ui->clothes->setRowCount(Clothes.size());
    for(unsigned int i=0;i<Clothes.size();i++)
    {
        ui->clothes->setItem(i,0,new QTableWidgetItem(Clothes[i].getName()));
        ui->clothes->setItem(i,1,new QTableWidgetItem(Clothes[i].getAttr()));
        ui->clothes->setItem(i,2,new QTableWidgetItem(Clothes[i].getDesc()));
        ui->clothes->setItem(i,3,new QTableWidgetItem(QString::number(Clothes[i].getRemain(),10)));
        ui->clothes->setItem(i,4,new QTableWidgetItem(QString::number(Clothes[i].product::getPrice(),'f',2)));
        ui->clothes->setItem(i,5,new QTableWidgetItem(QString::number(Clothes[i].getDiscount(),'f',2)));
        ui->clothes->setItem(i,6,new QTableWidgetItem(QString::number(Clothes[i].getPrice(),'f',2)));
        ui->clothes->QTableView::resizeRowsToContents();
    }
    ui->clothes->QTableView::resizeRowsToContents();
    ui->clothes->update();
    data.close();
}

void MainWindow::on_BindBtn_clicked()
{
    mycard mc;
    mc.getUser(login_user);
    mc.Init();
    mc.exec();
}



void MainWindow::on_SearchBtn_clicked()
{
    searchBook.clear();
    searchFood.clear();
    searchClothes.clear();
    QString search_str=ui->search->text();
    search_str.contains(search_str);
    if(search_str=="")
    {
        InitBook();
        InitFood();
        InitClothes();
    }
    else
    {
        int page=ui->tabWidget->currentIndex();
        if(page==0)
        {
            InitFood();
            InitClothes();
            for(unsigned int i=0;i<Book.size();i++)
            {
                if(Book[i].getName().contains(search_str))
                {
                    searchBook.push_back(Book[i]);
                }
            }
            ui->book->clear();
            ui->book->setEditTriggers(QAbstractItemView::NoEditTriggers);   //表格禁止编辑
            ui->book->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式
           //设置行列数
            ui->book->setColumnCount(7);
            ui->book->setRowCount(1);
           //设置每列宽
            for(int i=0;i<7;i++)
            {
                ui->book->setColumnWidth(i,109);
            }
            ui->book->setColumnWidth(2,600);
            //设置表头
            QStringList header;
            header.append("书名");
            header.append("作者");
            header.append("简介");
            header.append("剩余量");
            header.append("原价");
            header.append("折扣");
            header.append("现价");
            ui->book->setHorizontalHeaderLabels(header);
            ui->book->setRowCount(1);//表格加一行
            for (int i=0;i<7;i++)
            {
                ui->book->setItem(1,i,new QTableWidgetItem(""));
            }
            //ui->book->QTableView::resizeRowsToContents();

            ui->book->setRowCount(searchBook.size());
            for(unsigned int i=0;i<searchBook.size();i++)
            {
                ui->book->setItem(i,0,new QTableWidgetItem(searchBook[i].getName()));
                ui->book->setItem(i,1,new QTableWidgetItem(searchBook[i].getAuthor()));
                ui->book->setItem(i,2,new QTableWidgetItem(searchBook[i].getDesc()));
                ui->book->setItem(i,3,new QTableWidgetItem(QString::number(searchBook[i].getRemain(),10)));
                ui->book->setItem(i,4,new QTableWidgetItem(QString::number(searchBook[i].product::getPrice(),'f',2)));
                ui->book->setItem(i,5,new QTableWidgetItem(QString::number(searchBook[i].getDiscount(),'f',2)));
                ui->book->setItem(i,6,new QTableWidgetItem(QString::number(searchBook[i].getPrice(),'f',2)));
                //QPushButton *m_button = new QPushButton("添加");
                //connect(m_button,SIGNAL(clicked()),this,SLOT(slot_append()));
                //ui->book->setCellWidget(i,7,m_button);
                ui->book->QTableView::resizeRowsToContents();
            }
            ui->book->QTableView::resizeRowsToContents();
            ui->book->update();
        }
        else if(page==1)
        {
            InitBook();
            InitClothes();
            for(unsigned int i=0;i<Food.size();i++)
            {
                if(Food[i].getName().contains(search_str))
                {
                    searchFood.push_back(Food[i]);
                }
            }
            ui->food->clear();
            ui->food->setEditTriggers(QAbstractItemView::NoEditTriggers);   //表格禁止编辑
            ui->food->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式
           //设置行列数
            ui->food->setColumnCount(7);
            ui->food->setRowCount(1);
           //设置每列宽
            for(int i=0;i<7;i++)
            {
                ui->food->setColumnWidth(i,109);
            }
            ui->food->setColumnWidth(2,600);
            //设置表头
            QStringList header;
            header.append("书名");
            header.append("作者");
            header.append("简介");
            header.append("剩余量");
            header.append("原价");
            header.append("折扣");
            header.append("现价");
            ui->food->setHorizontalHeaderLabels(header);
            ui->food->setRowCount(1);//表格加一行
            for (int i=0;i<7;i++)
            {
                ui->food->setItem(1,i,new QTableWidgetItem(""));
            }
            //ui->food->QTableView::resizeRowsToContents();

            ui->food->setRowCount(searchFood.size());
            for(unsigned int i=0;i<searchFood.size();i++)
            {
                ui->food->setItem(i,0,new QTableWidgetItem(searchFood[i].getName()));
                ui->food->setItem(i,1,new QTableWidgetItem(searchFood[i].getMaintain()));
                ui->food->setItem(i,2,new QTableWidgetItem(searchFood[i].getDesc()));
                ui->food->setItem(i,3,new QTableWidgetItem(QString::number(searchFood[i].getRemain(),10)));
                ui->food->setItem(i,4,new QTableWidgetItem(QString::number(searchFood[i].product::getPrice(),'f',2)));
                ui->food->setItem(i,5,new QTableWidgetItem(QString::number(searchFood[i].getDiscount(),'f',2)));
                ui->food->setItem(i,6,new QTableWidgetItem(QString::number(searchFood[i].getPrice(),'f',2)));
                //QPushButton *m_button = new QPushButton("添加");
                //connect(m_button,SIGNAL(clicked()),this,SLOT(slot_append()));
                //ui->fook->setCellWidget(i,7,m_button);
                ui->food->QTableView::resizeRowsToContents();
            }
            ui->food->QTableView::resizeRowsToContents();
            ui->food->update();
        }
        else if(page==2)
        {
            InitBook();
            InitFood();
            for(unsigned int i=0;i<Clothes.size();i++)
            {
                if(Clothes[i].getName().contains(search_str))
                {
                    searchClothes.push_back(Clothes[i]);
                }
            }
            ui->clothes->clear();
            ui->clothes->setEditTriggers(QAbstractItemView::NoEditTriggers);   //表格禁止编辑
            ui->clothes->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式
           //设置行列数
            ui->clothes->setColumnCount(7);
            ui->clothes->setRowCount(1);
           //设置每列宽
            for(int i=0;i<7;i++)
            {
                ui->clothes->setColumnWidth(i,109);
            }
            ui->clothes->setColumnWidth(2,600);
            //设置表头
            QStringList header;
            header.append("书名");
            header.append("作者");
            header.append("简介");
            header.append("剩余量");
            header.append("原价");
            header.append("折扣");
            header.append("现价");
            ui->clothes->setHorizontalHeaderLabels(header);
            ui->clothes->setRowCount(1);//表格加一行
            for (int i=0;i<7;i++)
            {
                ui->clothes->setItem(1,i,new QTableWidgetItem(""));
            }
            //ui->clothes->QTableView::resizeRowsToContents();

            ui->clothes->setRowCount(searchClothes.size());
            for(unsigned int i=0;i<searchClothes.size();i++)
            {
                ui->clothes->setItem(i,0,new QTableWidgetItem(searchClothes[i].getName()));
                ui->clothes->setItem(i,1,new QTableWidgetItem(searchClothes[i].getAttr()));
                ui->clothes->setItem(i,2,new QTableWidgetItem(searchClothes[i].getDesc()));
                ui->clothes->setItem(i,3,new QTableWidgetItem(QString::number(searchClothes[i].getRemain(),10)));
                ui->clothes->setItem(i,4,new QTableWidgetItem(QString::number(searchClothes[i].product::getPrice(),'f',2)));
                ui->clothes->setItem(i,5,new QTableWidgetItem(QString::number(searchClothes[i].getDiscount(),'f',2)));
                ui->clothes->setItem(i,6,new QTableWidgetItem(QString::number(searchClothes[i].getPrice(),'f',2)));
                //QPushButton *m_button = new QPushButton("添加");
                //connect(m_button,SIGNAL(clicked()),this,SLOT(slot_append()));
                //ui->fook->setCellWidget(i,7,m_button);
                ui->clothes->QTableView::resizeRowsToContents();
            }
            ui->clothes->QTableView::resizeRowsToContents();
            ui->clothes->update();
        }
    }
}
