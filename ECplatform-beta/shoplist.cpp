#include "shoplist.h"
#include "ui_shoplist.h"

shoplist::shoplist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::shoplist)
{
    ui->setupUi(this);
    setFixedSize(711,419);
    this->setWindowTitle("购物车");
}

shoplist::~shoplist()
{
    delete ui;
}

void shoplist::setUser(User tmp)
{
    login_user=tmp;

}

void shoplist::setTominus(double x,double y)
{
    fulfill=x;
    tominus=y;
}

void shoplist::setDiscnt(double x)
{
    discount=x;
}

void shoplist::add()
{
    login_user.clearList();
    QSqlDatabase data;
    data=QSqlDatabase::addDatabase("QSQLITE");
    data.setDatabaseName("lists.db");
    if(!data.open())
        qDebug() << "Error:Failed to connect BankData" << data.lastError();
    else
    {
        QSqlQuery query(data);
        QString create_info="CREATE TABLE s"+login_user.getId()+" \
                            (name varchar(100),\
                            type varchar(100),\
                            describe varchar(2000),\
                            price double,\
                            count int,\
                             total double,\
                             primary key (name,type,describe))";
        query.prepare(create_info);
        if(!query.exec())
        {
            qDebug() << "Error: Fail to create lists." << query.lastError();
        }
        else
        {
            qDebug() << "Table created";
        }
        QString select_info="SELECT * FROM s"+login_user.getId();
        if(!query.exec(select_info))
            qDebug()<<query.lastError();
        else
        {
            while(query.next())
            {
                QString name=query.value(0).toString();
                QString type=query.value(1).toString();
                QString desc=query.value(2).toString();
                double price=query.value(3).toDouble();
                int count=query.value(4).toInt();
                double total=query.value(5).toDouble();
                list.setName(name);
                list.setType(type);
                list.setDesc(desc);
                list.setPrice(price);
                list.setCount(count);
                list.setTotal(total);
                login_user.pushList(list);
            }
        }
    }
    data.close();
}

void shoplist::Init()
{
    double total=0;
    ui->list->setWindowTitle(QObject::tr("购物车"));  //表名
    ui->list->setEditTriggers(QAbstractItemView::NoEditTriggers);   //表格禁止编辑
    ui->list->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式
   //设置行列数
    ui->list->setColumnCount(6);
    ui->list->setRowCount(1);
   //设置每列宽
    for (int i=0;i<6;i++)
    {
        ui->list->setColumnWidth(i,100);
    }
    //设置表头
    ui->list->setColumnWidth(2,190);
    QStringList header;
    header.append("商品");
    header.append("类型");
    header.append("描述");
    header.append("单价");
    header.append("数量");
    header.append("总价");
    ui->list->setHorizontalHeaderLabels(header);
    ui->list->setRowCount(1);//表格加一行
    for (int i=0;i<6;i++)
    {
        ui->list->setItem(1,i,new QTableWidgetItem(""));
    }
    ui->list->setRowCount(login_user.sizeList());
    for(unsigned int i=0;i<login_user.sizeList();i++)
    {
        ui->list->setItem(i,0,new QTableWidgetItem(login_user.selectList(i).getName()));
        ui->list->setItem(i,1,new QTableWidgetItem(login_user.selectList(i).getType()));
        ui->list->setItem(i,2,new QTableWidgetItem(login_user.selectList(i).getDesc()));
        ui->list->setItem(i,3,new QTableWidgetItem(QString::number(login_user.selectList(i).getPrice(),'f',2)));
        ui->list->setItem(i,4,new QTableWidgetItem(QString::number(login_user.selectList(i).getCount(),10)));
        ui->list->setItem(i,5,new QTableWidgetItem(QString::number(login_user.selectList(i).getTotal(),'f',2)));
        //QPushButton *m_button = new QPushButton("移除");
        //connect(m_button,SIGNAL(clicked()),this,SLOT(slot_append()));
        //ui->list->setCellWidget(i,5,m_button);
        ui->list->QTableView::resizeRowsToContents();
        total+=login_user.selectList(i).getTotal();
    }
    ui->list->QTableView::resizeRowsToContents();
    ui->list->update();
    ui->label->setText("");
    if(fulfill==0 || tominus==0)
    {

    }
    else
    {

        ui->label->setText("当前进行活动：单笔订单满"+QString::number(fulfill,'f',2)+"减"+QString::number(tominus,'f',2));
        if(total>=fulfill)
        {
            total-=tominus;
        }

    }
    ui->label_2->setText("所需付款总金额为："+QString::number(total,'f',2));
    topay=total;
}

bool shoplist::addIn(user_list tmp,int remain)
{
    QSqlDatabase data;
    data=QSqlDatabase::addDatabase("QSQLITE");
    data.setDatabaseName("lists.db");
    if(!data.open())
    {
        qDebug() << "Error:Failed to connect lists.db" << data.lastError();
        QMessageBox::information(NULL,"Error","打开购物车数据库lists.db失败",QMessageBox::Yes);
        return false;
    }
    else
    {
        QSqlQuery query(data);
        QString create_info="CREATE TABLE s"+login_user.getId()+" \
                            (name varchar(100),\
                            type varchar(100),\
                            describe varchar(2000),\
                            price double,\
                            count int,\
                            total double,\
                            primary key (name,type,describe))";
        query.prepare(create_info);
        if(!query.exec())
        {
            qDebug() << "Error: Fail to create lists." << query.lastError();
        }
        else
        {
            qDebug() << "Table created";
        }
        QString insert_info="INSERT INTO s"+login_user.getId()+" VALUES (?,?,?,?,?,?)";
        query.prepare(insert_info);
        query.addBindValue(tmp.getName());
        query.addBindValue(tmp.getType());
        query.addBindValue(tmp.getDesc());
        query.addBindValue(tmp.getPrice());
        query.addBindValue(tmp.getCount());
        query.addBindValue(tmp.getTotal());
        if(!query.exec())
        {
            QString select_info="SELECT count,price from s"+login_user.getId()+" WHERE name==\""+tmp.getName()+"\""+" AND type==\""+tmp.getType()+"\""+" AND describe==\""+tmp.getDesc()+"\"";
            query.exec(select_info);
            query.next();
            int tmp_cnt=query.value(0).toInt();
            double tmp_price=query.value(1).toDouble();
            if(tmp_cnt+tmp.getCount() > remain)
            {
                QMessageBox::information(NULL,"Error","该商品最多只能添加"+QString::number(remain,10)+"件到购物车，请检查购物车中该商品数量",QMessageBox::Yes);
                return false;
            }
            QString update_info="UPDATE s"+login_user.getId()+" SET count=:count,total=:total WHERE name==\""+tmp.getName()+"\""+" AND type==\""+tmp.getType()+"\""+" AND describe==\""+tmp.getDesc()+"\"";
            query.prepare(update_info);
            query.bindValue(":count",tmp_cnt+tmp.getCount());
            query.bindValue(":total",tmp_price*(tmp_cnt+tmp.getCount()));
            if(!query.exec())
            {
                qDebug()<<query.lastError();
                QMessageBox::information(NULL,"Error","Failed to update count",QMessageBox::Yes);
                return false;
            }
            return true;
        }
        else
        {
            qDebug()<<"insert success";
            return true;
        }
    }
}

void shoplist::on_RemoveBtn_clicked()
{
    bool focus=ui->list->isItemSelected(ui->list->currentItem());
    int row;
    if(focus)
    {
        row=ui->list->currentItem()->row();
    }
    else
    {
        QMessageBox::information(NULL,"Error","请选择要移出购物车的商品",QMessageBox::Yes);
        return;
    }
    QString _name=ui->list->item(row,0)->text();
    QString _type=ui->list->item(row,1)->text();
    QString _desc=ui->list->item(row,2)->text();
    QString _price=ui->list->item(row,3)->text();
    QString _count=ui->list->item(row,4)->text();
    QString _total=ui->list->item(row,5)->text();
    QSqlDatabase data;
    data=QSqlDatabase::addDatabase("QSQLITE");
    data.setDatabaseName("lists.db");
    if(!data.open())
    {
        qDebug() << "Error:Failed to connect lists.db" << data.lastError();
        QMessageBox::information(NULL,"Error","打开购物车数据库lists.db失败",QMessageBox::Yes);
        return;
    }
    else
    {
        QSqlQuery query(data);
        QString delete_info="DELETE FROM s"+login_user.getId()+" WHERE name==\""+_name+"\""+" AND type==\""+_type+"\""+" AND describe==\""+_desc+"\"";
        query.prepare(delete_info);
        if(!query.exec())
            qDebug()<<query.lastError();
        else
        {
            //login_user.deleteList(row);
            data.close();
            add();
            QMessageBox::information(NULL,"Success","移除成功",QMessageBox::Yes);
            Init();
        }
    }
}

void shoplist::on_ConfirmBtn_clicked()
{
    QSqlDatabase data,data2;
    QString record,record1;
    bool flag=true;
    int row=ui->list->rowCount();
    if(row==0)
    {
        QMessageBox::information(NULL,"Error","购物车空空如也~，请添加商品到购物车中",QMessageBox::Yes);
        return;
    }
    data=QSqlDatabase::addDatabase("QSQLITE","list");
    data.setDatabaseName("lists.db");
    data2=QSqlDatabase::addDatabase("QSQLITE","products");
    data2.setDatabaseName("products.db");
    if(!data.open())
    {
        qDebug() << "Error:Failed to connect lists.db" << data.lastError();
        QMessageBox::information(NULL,"Error","打开购物车数据库lists.db失败",QMessageBox::Yes);
        return;
    }
    if(!data2.open())
    {
        qDebug() << "Error:Failed to connect lists.db" << data.lastError();
        QMessageBox::information(NULL,"Error","打开商品数据库products.db失败",QMessageBox::Yes);
        return;
    }
    for(int i=0;i<row;i++)
    {
        QString _name=ui->list->item(i,0)->text();
        QString _type=ui->list->item(i,1)->text();
        QString _desc=ui->list->item(i,2)->text();
        QString _price=ui->list->item(i,3)->text();
        QString _count=ui->list->item(i,4)->text();
        QString _total=ui->list->item(i,5)->text();
        QSqlQuery query(data),query_product(data2);
        if(_type=="图书")
        {
            QString select_product="SELECT remain FROM Book WHERE name==\""+_name+"\""+" AND describe==\""+_desc+"\"";
            query_product.exec(select_product);
            query_product.next();
            int tmp_remain=query_product.value(0).toInt();
            QString select_info="SELECT count FROM s"+login_user.getId()+" WHERE name==\""+_name+"\""+" AND type==\""+_type+"\""+" AND describe==\""+_desc+"\"";
            query.exec(select_info);
            query.next();
            int tmp_product_remain=query.value(0).toInt();
            if(tmp_remain<tmp_product_remain)
            {
                flag=false;
                record=_name;
                record1="图书";
                break;
            }
        }
        else if(_type=="食品")
        {
            QString select_product="SELECT remain FROM Food WHERE name==\""+_name+"\""+" AND describe==\""+_desc+"\"";
            query_product.exec(select_product);
            query_product.next();
            int tmp_remain=query_product.value(0).toInt();
            QString select_info="SELECT count FROM s"+login_user.getId()+" WHERE name==\""+_name+"\""+" AND type==\""+_type+"\""+" AND describe==\""+_desc+"\"";
            query.exec(select_info);
            query.next();
            int tmp_product_remain=query.value(0).toInt();

            if(tmp_remain<tmp_product_remain)
            {
                flag=false;
                record=_name;
                record1="食品";
                break;
            }
        }
        else if(_type=="服装")
        {
            QString select_product="SELECT remain FROM Clothes WHERE name==\""+_name+"\""+" AND describe==\""+_desc+"\"";
            query_product.exec(select_product);
            query_product.next();
            int tmp_remain=query_product.value(0).toInt();
            QString select_info="SELECT count FROM s"+login_user.getId()+" WHERE name==\""+_name+"\""+" AND type==\""+_type+"\""+" AND describe==\""+_desc+"\"";
            query.exec(select_info);
            query.next();
            int tmp_product_remain=query.value(0).toInt();
            if(tmp_remain<tmp_product_remain)
            {
                flag=false;
                record=_name;
                record1="服装";
                break;
            }
        }
    }
    if(!flag)
    {
        QMessageBox::information(NULL,"Error","当前购物车中"+record1+"类"+record+"超过库存上限",QMessageBox::Yes);
        return;
    }
    mycard mc;
    mc.getUser(login_user);
    mc.changeAfford(true);
    mc.Init();
    mc.setTotal(topay);
    qDebug()<<"shoplist "<<QString::number(topay,'f',2);
    if(mc.exec() == QDialog::Accepted)
    {
        for(int i=0;i<row;i++)
        {
            QString _name=ui->list->item(i,0)->text();
            QString _type=ui->list->item(i,1)->text();
            QString _desc=ui->list->item(i,2)->text();
            QString _price=ui->list->item(i,3)->text();
            QString _count=ui->list->item(i,4)->text();
            QString _total=ui->list->item(i,5)->text();
            QSqlQuery query(data);
            //QString delete_info="SELECT FROM s"+login_user.getId()+" WHERE name==\""+_name+"\""+" AND type==\""+_type+"\""+" AND describe==\""+_desc+"\"";
            QString delete_info="DELETE FROM s"+login_user.getId()+" WHERE name==\""+_name+"\""+" AND type==\""+_type+"\""+" AND describe==\""+_desc+"\"";
            query.prepare(delete_info);
            if(!query.exec())
            {
                qDebug()<<query.lastError();
                return;
            }
            user_list tmp;
            tmp.setName(_name);
            tmp.setType(_type);
            tmp.setDesc(_desc);
            tmp.setCount(_count.toInt());
            emit send_result(tmp);
        }
        add();
        Init();
    }
}

void shoplist::on_CancelBtn_clicked()
{
    this->close();
}
