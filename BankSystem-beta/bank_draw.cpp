#include "bank_draw.h"
#include "ui_bank_draw.h"
#include <QMessageBox>
#include <QDebug>
bank_draw::bank_draw(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::bank_draw)
{
    ui->setupUi(this);
    this->setWindowTitle("取款");
}

bank_draw::~bank_draw()
{
    delete ui;
}

void bank_draw::GetInfo(Bank t)
{
    login_user=t;
    ui->remain->setText(QString::number(login_user.GetRemain(),'f',2));

}

void bank_draw::on_ConfirmBtn_clicked()
{
    QString draw=ui->draw->text();
    if(draw=="")
    {
        QMessageBox::information(NULL,"Error","请输入取款金额",QMessageBox::Yes);
        return;
    }
    for(int i=0;i<draw.length();i++)
    {
        if(!((draw[i]<='9' && draw[i]>='0') || draw[i]=='.'))
        {
            qDebug()<<i<<" "<<draw[i];

            QMessageBox::information(NULL,"Error","输入取款金额错误，只能为整数或合法小数",QMessageBox::Yes);
            return;
        }
        else if(draw[i]=='.')
        {
            if(i==0)
            {
                QMessageBox::information(NULL,"Error","输入取款金额错误，只能为整数或合法小数",QMessageBox::Yes);
                return;
            }
            else if(i==(draw.length()-1))
            {
                QMessageBox::information(NULL,"Error","输入取款金额错误，只能为整数或合法小数",QMessageBox::Yes);
                return;
            }
        }
    }
    if(draw.length()>8 || draw.toDouble()>=100000)
    {
        QMessageBox::information(NULL,"Error","每次取款不能超过10万",QMessageBox::Yes);
        return;
    }
    double tmp=login_user.GetRemain();
    qDebug() << draw.toDouble();
    if(draw.toDouble() > tmp)
    {
        QMessageBox::information(NULL,"Error","当前账户余额不足，请减少所需取款金额",QMessageBox::Yes);
        return;
    }
    tmp-=draw.toDouble();
    Bank t=login_user;
    t.Update(QString::number(tmp,'f',2),1);
    login_user=t;
    ui->remain->setText(QString::number(login_user.GetRemain(),'f',2));
    QMessageBox::information(NULL,"Success","取款成功",QMessageBox::Yes);
    this->accept();
}

void bank_draw::on_CancelBtn_clicked()
{
    this->close();
}
