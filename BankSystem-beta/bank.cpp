#include "bank.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
Bank::Bank()
{
    user="";
    id="";
    card_id="";
    passwd="";
    bank_type="中国工商银行";
    remain=0;
}

void Bank::LoginBank(QString _card_id,QString _passwd,QString _bank_type)
{
    card_id=_card_id;
    passwd=_passwd;
    bank_type=_bank_type;
    qDebug() << _bank_type;
}

bool Bank::RegisterBank(QString _user,QString _id,QString _card_id,QString _passwd,QString _bank)
{
    if(_user=="" || _id=="" || _card_id=="" || _passwd=="")
    {
        QMessageBox::information(NULL,"Error","任何一项不能为空",QMessageBox::Yes);
        return false;
    }
    if(_passwd.length() != 6)
    {
        QMessageBox::information(NULL,"Error","银行卡密码必须为6位数字",QMessageBox::Yes);
        return false;
    }
    for(int i=0;i<_passwd.length();i++)
        if(!(_passwd[i]<='9' && _passwd[i]>='0'))
        {
            QMessageBox::information(NULL,"Error","银行卡密码必须为6位数字",QMessageBox::Yes);
            return false;
        }
    if(_id.length() != 18)
    {
        QMessageBox::information(NULL,"Error","身份证必须为18位",QMessageBox::Yes);
        return false;
    }
    for(int i=0;i<_user.length();i++)
        if(_user[i]<='9' && _user[i]>='0')
        {
            QMessageBox::information(NULL,"Error","持卡人姓名输入错误",QMessageBox::Yes);
            return false;
        }
    for(int i=0;i<_id.length();i++)
        if(!(_id[i]<='9' && _id[i]>='0'))
        {
            if(i == _id.length()-1)
            {
                qDebug()<<(_id[i]=='X');
                if((_id[i]<='9' && _id[i]>='0') || _id[i]=='X' || _id[i]=='x')
                    continue;
                else
                {
                    QMessageBox::information(NULL,"Error","身份证输入错误",QMessageBox::Yes);
                    return false;
                }
            }
            QMessageBox::information(NULL,"Error","身份证输入错误",QMessageBox::Yes);
            return false;
        }
    for(int i=0;i<_card_id.length();i++)
        if(!(_card_id[i]<='9' && _card_id[i]>='0'))
        {
            QMessageBox::information(NULL,"Error","银行卡号输入错误",QMessageBox::Yes);
            return false;
        }
    if(_card_id.size()<6)
    {
        QMessageBox::information(NULL,"Error","银行卡号必须大于等于6位",QMessageBox::Yes);
        return false;
    }
    user=_user;
    id=_id;
    card_id=_card_id;
    passwd=_passwd;
    bank_type=_bank;
    remain=0;
    return true;
}

bool Bank::CheckLogin()
{
    QSqlDatabase data;
    data=QSqlDatabase::addDatabase("QSQLITE");
    data.setDatabaseName("BankData.db");
    bool flag=false;
    if(!data.open())
        qDebug() << "Error in Check:Failed to connect BankData" << data.lastError();
    else
    {
        QSqlQuery query_bank(data);
        QString select_info="SELECT * from BankSystem WHERE card_id=="+card_id;
        if(!query_bank.exec(select_info))
            qDebug()<<query_bank.lastError();
        else
        {
            while(query_bank.next())
            {
                QString tmp_user,tmp_passwd,tmp_bank;
                tmp_user=query_bank.value(0).toString();
                tmp_passwd=query_bank.value(1).toString();
                tmp_bank=query_bank.value(2).toString();
                qDebug()<<tmp_user<<tmp_passwd<<tmp_bank;
                if(tmp_user==card_id && tmp_passwd==passwd && tmp_bank==bank_type)
                {
                    flag=true;
                    user=query_bank.value(3).toString();
                    id=query_bank.value(4).toString();
                    remain=query_bank.value(5).toDouble();
                    break;
                }
            }
        }
    }
    data.close();
    if(flag)
        return true;
    else
        return false;
}

void Bank::Registered()
{
    QSqlDatabase data;
    data=QSqlDatabase::addDatabase("QSQLITE");
    data.setDatabaseName("BankData.db");
    if(!data.open())
        qDebug() << "Error:Failed to connect BankData" << data.lastError();
    else
    {
        QSqlQuery query_bank(data);

        qDebug() << QSqlDatabase::drivers();
        QString create_sql="CREATE TABLE BankSystem\
                            (card_id int NOT NULL,\
                            card_passwd int NOT NULL,\
                            bank_name varchar(30) NOT NULL,\
                            user varchar(30) NOT NULL,\
                            user_id int,\
                            remain double,\
                            primary key (card_id,bank_name))";
        query_bank.prepare(create_sql);
        if(!query_bank.exec())
        {
            //qDebug() << "Error: Fail to create BankSystem." << query_bank.lastError();
        }
        else
        {
            qDebug() << "Table created";
        }

        //可选银行
        create_sql="CREATE TABLE BankName (name varchar(30) NOT NULL PRIMARY KEY)";
        query_bank.prepare(create_sql);
        query_bank.exec();
        query_bank.exec("INSERT INTO BankName VALUES (\"中国工商银行\")");
        query_bank.exec("INSERT INTO BankName VALUES (\"中国建设银行\")");
        query_bank.exec("INSERT INTO BankName VALUES (\"中国银行\")");
        query_bank.exec("INSERT INTO BankName VALUES (\"交通银行\")");
        query_bank.exec("INSERT INTO BankName VALUES (\"中国农业银行\")");
        query_bank.exec("INSERT INTO BankName VALUES (\"招商银行\")");
        query_bank.exec("INSERT INTO BankName VALUES (\"邮政储蓄银行\")");
        query_bank.exec("INSERT INTO BankName VALUES (\"光大银行\")");
        query_bank.exec("INSERT INTO BankName VALUES (\"民生银行\")");
    }
    data.close();
}

void Bank::Update(QString tmp,int n)
{
    QSqlDatabase data;
    data=QSqlDatabase::addDatabase("QSQLITE");
    data.setDatabaseName("BankData.db");
    if(!data.open())
        qDebug() << "Error:Failed to connect BankData" << data.lastError();
    else
    {
        QSqlQuery query(data);
        QString update_info="UPDATE BankSystem SET card_passwd=:card_passwd,remain=:remain WHERE card_id=="+card_id+" AND bank_name==\""+bank_type+"\"";
                qDebug()<<update_info;
        query.prepare(update_info);
        if(n == 1)
        {
            remain=tmp.toDouble();
            query.bindValue(":card_passwd",passwd);
            query.bindValue(":remain",remain);
            if(!query.exec())
                QMessageBox::information(NULL,"Error","Failed to update remain",QMessageBox::Yes);
        }
        else if(n == 2)
        {
            passwd=tmp;
            query.bindValue(":card_passwd",passwd);
            query.bindValue(":remain",remain);
            if(!query.exec())
                QMessageBox::information(NULL,"Error","Failed to update passwd",QMessageBox::Yes);
        }
    }
    data.close();
}
