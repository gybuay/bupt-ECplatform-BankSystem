#include "bank_deposit.h"
#include "ui_bank_deposit.h"
#include <QMessageBox>
#include <QDebug>
bank_deposit::bank_deposit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::bank_deposit)
{
    ui->setupUi(this);
    this->setWindowTitle("存款");
}

bank_deposit::~bank_deposit()
{
    delete ui;
}

void bank_deposit::GetInfo(Bank t)
{
    login_user=t;
    ui->remain->setText(QString::number(login_user.GetRemain(),'f',2));

}

void bank_deposit::on_ConfirmBtn_clicked()
{
    QString deposit=ui->deposit->text();
    if(deposit=="")
    {
        QMessageBox::information(NULL,"Error","请输入存款金额",QMessageBox::Yes);
        return;
    }
    for(int i=0;i<deposit.length();i++)
    {
        if(!(deposit[i]<='9' && deposit[i]>='0' || deposit[i]=='.'))
        {
            qDebug()<<i<<" "<<deposit[i];

            QMessageBox::information(NULL,"Error","输入存款金额错误，只能为整数或合法小数",QMessageBox::Yes);
            return;
        }
        else if(deposit[i]=='.')
        {
            if(i==0)
            {
                QMessageBox::information(NULL,"Error","输入存款金额错误，只能为整数或合法小数",QMessageBox::Yes);
                return;
            }
            else if(i==(deposit.length()-1))
            {
                QMessageBox::information(NULL,"Error","输入存款金额错误，只能为整数或合法小数",QMessageBox::Yes);
                return;
            }
        }
    }
    if(deposit.length()>8 || deposit.toDouble()>=100000)
    {
        QMessageBox::information(NULL,"Error","每次存款不能超过10万",QMessageBox::Yes);
        return;
    }
    double tmp=login_user.GetRemain();
    tmp+=deposit.toDouble();
    qDebug() << tmp;
    Bank t=login_user;
    t.Update(QString::number(tmp,'f',2),1);
    qDebug()<<"aa"<<t.GetRemain();
    login_user=t;
    ui->remain->setText(QString::number(login_user.GetRemain(),'f',2));
    QMessageBox::information(NULL,"Success","存款成功",QMessageBox::Yes);
    this->accept();
}

void bank_deposit::on_CancelBtn_clicked()
{
    this->close();
}
