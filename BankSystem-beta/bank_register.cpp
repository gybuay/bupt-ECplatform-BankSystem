#include "bank_register.h"
#include "ui_bank_register.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include "bank.h"
bank_register::bank_register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::bank_register)
{
    ui->setupUi(this);
    this->setWindowTitle("账户注册");
    ui->id->setPlaceholderText("18位");
    ui->name->setPlaceholderText("中文或英文");
    ui->passwd->setPlaceholderText("6位数字");
    ui->card_id->setPlaceholderText("至少6位数字");
    ui->ConfirmBtn->setFocus();
    ui->ConfirmBtn->setDefault(true);
    ui->name->setFocus();
    QSqlDatabase data;
    data=QSqlDatabase::addDatabase("QSQLITE");
    data.setDatabaseName("BankData.db");
    if(!data.open())
        QMessageBox::information(NULL,"Error","Failed to connect BankData",QMessageBox::Yes);
    else
    {
        QSqlQuery query(data);
        QString select_info="SELECT name from BankName";
        if(!query.exec(select_info))
        {
            //QMessageBox::information(NULL,"Error","Failed to execute select",QMessageBox::Yes);
            return;
        }
        else
        {
            while(query.next())
            {
                QString tmp_name;
                tmp_name=query.value(0).toString();
                ui->bank->addItem(tmp_name);
            }
        }
    }
    data.close();
}

bank_register::~bank_register()
{
    delete ui;
}

void bank_register::on_ConfirmBtn_clicked()
{
    QString tmp_name,tmp_card_id,tmp_id,tmp_passwd,tmp_bank;
    tmp_name=ui->name->text();
    tmp_card_id=ui->card_id->text();
    tmp_id=ui->id->text();
    tmp_passwd=ui->passwd->text();
    tmp_bank=ui->bank->currentText();

    Bank t;
    if(t.RegisterBank(tmp_name,tmp_id,tmp_card_id,tmp_passwd,tmp_bank))
    {
        QSqlDatabase data;
        data=QSqlDatabase::addDatabase("QSQLITE");
        data.setDatabaseName("BankData.db");
        if(!data.open())
            QMessageBox::information(NULL,"Error","Failed to connect BankData",QMessageBox::Yes);
        else
        {
            QSqlQuery query(data);
            /*QString select_info="SELECT card_id,bank_name from BankSystem";
            if(!query.exec(select_info))
                qDebug()<<query.lastError();
            else
            {
                while(query.next())
                {

                    QString select_card=query.value(0).toString();
                    QString select_bank=query.value(1).toString();
                    if(select_bank==tmp_bank && select_card==tmp_card_id)
                    {
                        QMessageBox::information(NULL,"Error","该银行对应此卡号已注册",QMessageBox::Yes);
                        return;
                    }
                }
            }*/
            QString insert_info="INSERT INTO BankSystem VALUES (?,?,?,?,?,?)";
            query.prepare(insert_info);
            query.addBindValue(tmp_card_id);
            query.addBindValue(tmp_passwd);
            query.addBindValue(tmp_bank);
            query.addBindValue(tmp_name);
            query.addBindValue(tmp_id);
            query.addBindValue(0);
            if(!query.exec())
            {
                QMessageBox::information(NULL,"Error","该银行对应此卡号已注册",QMessageBox::Yes);
                data.close();
            }
            else
            {
                QMessageBox::information(NULL,"Success","注册成功",QMessageBox::Yes);
                data.close();
                this->accept();
            }
        }

    }
}

void bank_register::on_CancelBtn_clicked()
{
    this->accept();
}
