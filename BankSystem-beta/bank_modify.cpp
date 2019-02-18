#include "bank_modify.h"
#include "ui_bank_modify.h"
#include <QMessageBox>
bank_modify::bank_modify(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::bank_modify)
{
    ui->setupUi(this);
    this->setWindowTitle("修改密码");
    ui->passwd0->setPlaceholderText("6位数字");
    ui->passwd1->setPlaceholderText("6位数字");
    ui->passwd0->setEchoMode(QLineEdit::Password);
    ui->passwd1->setEchoMode(QLineEdit::Password);
}

bank_modify::~bank_modify()
{
    delete ui;
}

void bank_modify::GetInfo(Bank t)
{
    login_user=t;
}

void bank_modify::on_CancelBtn_clicked()
{
    this->accept();
}

void bank_modify::on_ConfirmBtn_clicked()
{
    QString passwd0=ui->passwd0->text();
    QString passwd1=ui->passwd1->text();
    if(passwd0 != passwd1)
    {
        QMessageBox::information(NULL,"Error","两次输入密码不一致,请重新输入",QMessageBox::Yes);
    }
    else
    {
        if(passwd0.length() != 6)
        {
            QMessageBox::information(NULL,"Error","银行卡密码必须为6位数字",QMessageBox::Yes);
            return;
        }
        for(int i=0;i<passwd0.length();i++)
            if(!(passwd0[i]<='9' && passwd0[i]>='0'))
            {
                QMessageBox::information(NULL,"Error","银行卡密码必须为6位数字",QMessageBox::Yes);
                return;
            }
        Bank t=login_user;
        t.Update(passwd0,2);
        QMessageBox::information(NULL,"Success","修改密码成功",QMessageBox::Yes);
        this->accept();
    }
}
