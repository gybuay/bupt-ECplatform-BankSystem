#include "mycard.h"
#include "ui_mycard.h"

mycard::mycard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mycard)
{
    ui->setupUi(this);
    this->setWindowTitle("我的银行卡");
    setFixedSize(663,383);
    changeAfford(false);
}

mycard::~mycard()
{
    delete ui;
}

void mycard::changeAfford(bool x)
{
    if(x)
    {
        ui->pushButton->show();
        ui->BindBtn->hide();
        ui->RemoveBtn->hide();
    }
    else
    {
        ui->pushButton->hide();
        ui->BindBtn->show();
        ui->RemoveBtn->show();
    }
}

void mycard::setTotal(double x)
{
    total=x;
}

void mycard::on_pushButton_clicked()//afford
{
    int row;
    bool focus=ui->cardlist->isItemSelected(ui->cardlist->currentItem());
    if(focus)
    {
        row=ui->cardlist->currentItem()->row();
    }
    else
    {
        QMessageBox::information(NULL,"Error","请选择需要用于支付的银行卡",QMessageBox::Yes);
        return;
    }
    afford aff;
    aff.setInfo(bank_type[row],card_id[row],total);
    qDebug()<<"mycard "<<QString::number(total,'f',2);
    if(aff.exec() == QDialog::Accepted)
    {
        this->accept();
    }
}

void mycard::getUser(User x)
{
    login_user=x;
}

void mycard::Init()
{
    bank_type.clear();
    card_id.clear();
    ui->cardlist->clear();
    QSqlDatabase data;
    data=QSqlDatabase::addDatabase("QSQLITE","cardcard");
    data.setDatabaseName("cards.db");
    if(!data.open())
        qDebug() << "Error:Failed to connect cards.db" << data.lastError();
    else
    {
        QSqlQuery query(data);
        QString create_info="CREATE TABLE c"+login_user.getId()+" \
                            (type varchar(100),\
                            card_id varchar(100),\
                             primary key (card_id,type))";
        query.prepare(create_info);
        if(!query.exec())
        {
            qDebug() << "Error: Fail to create cards." << query.lastError();
        }
        else
        {
            qDebug() << "Table created";
        }
        QString select_info="SELECT * FROM c"+login_user.getId();
        if(!query.exec(select_info))
        {
            qDebug()<<query.lastError();
            qDebug()<<"query.lastError()";
            return;
        }
        while(query.next())
        {
            QString type=query.value(0).toString();
            QString tmp_card_id=query.value(1).toString();
            bank_type.push_back(type);
            card_id.push_back(tmp_card_id);
        }


        ui->cardlist->setWindowTitle(QObject::tr("银行卡"));  //表名
        ui->cardlist->setEditTriggers(QAbstractItemView::NoEditTriggers);   //表格禁止编辑
        ui->cardlist->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式
       //设置行列数
        ui->cardlist->setColumnCount(2);
        ui->cardlist->setRowCount(1);
       //设置每列宽
        for (int i=0;i<2;i++)
        {
            ui->cardlist->setColumnWidth(i,258);
        }
        //设置表头
        QStringList header;
        header.append("所属银行");
        header.append("卡号");
        ui->cardlist->setHorizontalHeaderLabels(header);
        ui->cardlist->setRowCount(1);//表格加一行
        for (int i=0;i<2;i++)
        {
            ui->cardlist->setItem(1,i,new QTableWidgetItem(""));
        }
        ui->cardlist->setRowCount(card_id.size());
        for(unsigned int i=0;i<card_id.size();i++)
        {
            ui->cardlist->setItem(i,0,new QTableWidgetItem(bank_type[i]));
            ui->cardlist->setItem(i,1,new QTableWidgetItem(card_id[i]));
            ui->cardlist->QTableView::resizeRowsToContents();
        }
        ui->cardlist->QTableView::resizeRowsToContents();
        ui->cardlist->update();
    }
}

void mycard::on_CancelBtn_clicked()
{
    this->close();
}


void mycard::receive_card(QString a,QString b)
{
    QSqlDatabase data;
    data=QSqlDatabase::addDatabase("QSQLITE","cardcard");
    data.setDatabaseName("cards.db");
    if(!data.open())
    {
        qDebug() << "Error:Failed to connect cards.db" << data.lastError();
        return;
    }
    QSqlQuery query(data);
    QString select_info="INSERT INTO c"+login_user.getId()+" VALUES (?,?)";
    query.prepare(select_info);
    query.addBindValue(b);
    query.addBindValue(a);
    if(!query.exec())
    {
        QMessageBox::information(NULL,"Error","您已绑定此卡",QMessageBox::Yes);
        return;
    }
    else
    {
        QMessageBox::information(NULL,"Success","绑定成功！",QMessageBox::Yes);
        Init();
    }
}

void mycard::on_BindBtn_clicked()
{
    bind_card bc;
    connect(&bc,SIGNAL(send_card(QString,QString)),this,SLOT(receive_card(QString,QString)));
    bc.exec();

}

void mycard::on_RemoveBtn_clicked()
{
    QSqlDatabase data;
    data=QSqlDatabase::addDatabase("QSQLITE","cardcard");
    data.setDatabaseName("cards.db");
    if(!data.open())
    {
        qDebug() << "Error:Failed to connect cards.db" << data.lastError();
        return;
    }
    int row;
    bool focus=ui->cardlist->isItemSelected(ui->cardlist->currentItem());
    if(focus)
    {
        row=ui->cardlist->currentItem()->row();
    }
    else
    {
        QMessageBox::information(NULL,"Error","请选择需要解绑的银行卡",QMessageBox::Yes);
        data.close();
        return;
    }
    QSqlQuery query(data);
    QString delete_info="DELETE FROM c"+login_user.getId()+" WHERE type==\""+bank_type[row]\
            +"\" AND card_id==\""+card_id[row]+"\"";
    qDebug()<<delete_info;
    query.prepare(delete_info);
    if(!query.exec())
    {
        qDebug()<<query.lastError();
        data.close();
        return;
    }
    else
    {
        QMessageBox::information(NULL,"Success","解绑成功",QMessageBox::Yes);
        data.close();
        Init();
        return;
    }
}
