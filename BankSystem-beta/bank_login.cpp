#include "bank_login.h"
#include "ui_bank_login.h"
#include "bank_deposit.h"
#include "bank_draw.h"
#include "bank_modify.h"
#include <QMessageBox>
#include<QDebug>
bank_login::bank_login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::bank_login)
{
    ui->setupUi(this);
    ui->DepositBtn->setFocus();
    ui->DepositBtn->setDefault(true);
    this->setWindowTitle("账户信息");
}

bank_login::~bank_login()
{
    delete ui;
}

void bank_login::receive_update(double x)
{
    qDebug()<<QString::number(x,'f',2);
    ui->remain->setText(QString::number(x,'f',2));
}

void bank_login::GetInfo(Bank t)
{
    login_user=t;
    ui->name->setText(login_user.GetUser());
    ui->bank->setText(login_user.GetBank());
    ui->remain->setText(QString::number(login_user.GetRemain(),'f',2));
}

void bank_login::on_DepositBtn_clicked()
{
    bank_deposit bde;
    bde.GetInfo(login_user);
    bde.exec();
    login_user=bde.PassInfo();
    ui->remain->setText(QString::number(login_user.GetRemain(),'f',2));
}

void bank_login::on_DrawBtn_clicked()
{
    bank_draw bdr;
    bdr.GetInfo(login_user);
    bdr.exec();
    login_user=bdr.PassInfo();
    ui->remain->setText(QString::number(login_user.GetRemain(),'f',2));
}

void bank_login::on_LogoutBtn_clicked()
{
    QMessageBox::information(NULL,"Success","注销成功",QMessageBox::Yes);
    this->accept();
}

void bank_login::on_ModifyBtn_clicked()
{
    bank_modify bm;
    bm.GetInfo(login_user);
    bm.exec();
    login_user=bm.PassInfo();
}
