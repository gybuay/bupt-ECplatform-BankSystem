#include "user.h"

User::User()
{

}

User::User(QString _id,QString _passwd,QString _mobile)
{
    id=_id;
    passwd=_passwd;
    mobile=_mobile;
}

/*void User::setList(QString name,QString type,double price,int count,double total)
{
    user_list tmp;
    tmp.setName(name);
    tmp.setType(type);
    tmp.setPrice(price);
    tmp.setCount(count);
    tmp.setTotal(total);
    list.push_back(tmp);
}*/

bool User::userInsert()
{
    QSqlDatabase data;
    data=QSqlDatabase::addDatabase("QSQLITE");
    data.setDatabaseName("users.db");
    if(!data.open())
    {
        qDebug() << "Error in Check:Failed to connect BankData" << data.lastError();
        return false;
    }
    else
    {
        QSqlQuery query;
        QString create_info="CREATE TABLE user (id varchar(50) PRIMARY KEY,\
                            passwd varchar(50),mobile varchar(50))";
        query.prepare(create_info);
        if(query.exec())
            qDebug()<<"Table created";
        QString insert_info="INSERT INTO user VALUES (?,?,?)";
        query.prepare(insert_info);
        query.addBindValue(id);
        query.addBindValue(passwd);
        query.addBindValue(mobile);
        if(!query.exec())
        {
            QMessageBox::information(NULL,"Error","该账号已被注册",QMessageBox::Yes);
            data.close();
            return false;
        }
        else
        {
            QMessageBox::information(NULL,"Success","注册成功",QMessageBox::Yes);
            data.close();
            return true;
        }
    }
}

bool User::userLogin()
{
    QSqlDatabase data;
    bool flag=false;
    data=QSqlDatabase::addDatabase("QSQLITE");
    data.setDatabaseName("users.db");
    if(!data.open())
    {
        qDebug() << "Error in Check:Failed to connect BankData" << data.lastError();
        return false;
    }
    else
    {
        QSqlQuery query;
        QString create_info="CREATE TABLE user (id varchar(50) PRIMARY KEY,\
                            passwd varchar(50),mobile varchar(50))";
        query.prepare(create_info);
        if(query.exec())
            qDebug()<<"Table created";
        QString select_info="SELECT * FROM user WHERE id==\""+id+"\"";
        if(!query.exec(select_info))
            qDebug()<<query.lastError();
        while(query.next())
        {
            QString tmp_id=query.value(0).toString();
            QString tmp_passwd=query.value(1).toString();
            if(tmp_id==id && tmp_passwd==passwd)
            {
                flag=true;
                mobile=query.value(2).toString();
                break;
            }
        }
        if(flag)
            return true;
        else
            return false;
    }
}
