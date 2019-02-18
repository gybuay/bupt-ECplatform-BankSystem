#include "product_minus.h"
#include "ui_product_minus.h"
#include <QMessageBox>
product_minus::product_minus(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::product_minus)
{
    ui->setupUi(this);
    setFixedSize(242,156);
    this->setWindowTitle("满减活动设置");
}

product_minus::~product_minus()
{
    delete ui;
}

void product_minus::on_ConfirmBtn_clicked()
{
    QString _fulfill=ui->fulfill->text();
    QString _tominus=ui->tominus->text();
    for(int i=0;i<_fulfill.size();i++)
    {
        if(!(_fulfill[i]>='0' && _fulfill[i]<='9'))
        {
            QMessageBox::information(NULL,"Error","设置数值为非负整数",QMessageBox::Yes);
            return;
        }
    }
    for(int i=0;i<_tominus.size();i++)
    {
        if(!(_tominus[i]>='0' && _tominus[i]<='9'))
        {
            QMessageBox::information(NULL,"Error","设置数值为非负整数",QMessageBox::Yes);
            return;
        }
    }
    if(_fulfill=="" || _tominus=="")
    {
        QMessageBox::information(NULL,"Error","任意一项不能为空",QMessageBox::Yes);
        return;
    }
    if(_fulfill=="" || _tominus=="")
    {
        QMessageBox::information(NULL,"Error","任意一项不能为空",QMessageBox::Yes);
        return;
    }
    double fulfill=_fulfill.toDouble();
    double tominus=_tominus.toDouble();
    if(fulfill <= tominus)
    {
        if(fulfill==0 && tominus==fulfill)
        {
            emit send_tominus(fulfill,tominus);
            QMessageBox::information(NULL,"Success","设置成功",QMessageBox::Yes);
            accept();
        }
        else
        {
            QMessageBox::information(NULL,"Error","优惠幅度过大，将会导致血亏，请重新设置",QMessageBox::Yes);
            return;
        }
    }
    else if(fulfill<0 || tominus<0)
    {
        QMessageBox::information(NULL,"Error","设置数值为非负整数",QMessageBox::Yes);
        return;
    }
    else
    {
        emit send_tominus(fulfill,tominus);
        QMessageBox::information(NULL,"Success","设置成功",QMessageBox::Yes);
        accept();
    }
}

void product_minus::on_CancelBtn_clicked()
{
    this->close();
}
