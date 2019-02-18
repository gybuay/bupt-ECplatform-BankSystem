#include "afford.h"
#include "ui_afford.h"

afford::afford(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::afford)
{
    ui->setupUi(this);
    setFixedSize(237,108);
    this->setWindowTitle("支付");
    ui->input->setEchoMode(QLineEdit::Password);
    socket=new QTcpSocket();
    connect(socket,&QTcpSocket::readyRead,this,&afford::read_bank);
    connect(socket,&QTcpSocket::disconnected,this,&afford::release_bank);
    socket->connectToHost(QHostAddress::LocalHost,26666);
    if(!socket->waitForConnected(30000))
    {
        QMessageBox::information(NULL,"Error","连接银行失败",QMessageBox::Yes);
        return;
    }
    else
    {
        //QMessageBox::information(NULL,"Success","连接银行成功",QMessageBox::Yes);
    }
}

afford::~afford()
{
    delete this->socket;
    delete ui;
}

void afford::setInfo(QString a,QString b,double c)
{
    bank_type=a;
    card_id=b;
    total=c;
}

void afford::read_bank()
{
    QString buffer;
    buffer = socket->readAll();
    qDebug()<<"in afford"<<buffer;
    if(!buffer.isEmpty())
    {
        if(buffer=="true in afford")
        {
            QMessageBox::information(NULL,"Success","支付成功",QMessageBox::Yes);
            this->accept();
        }
        else if(buffer=="false in afford")
        {
            QMessageBox::information(NULL,"Error","支付失败",QMessageBox::Yes);
            this->close();
        }
        else if(buffer=="false in afford remain")
        {
            QMessageBox::information(NULL,"Error","支付失败，该银行卡余额不足",QMessageBox::Yes);
            this->close();
        }
        else if(buffer=="false in afford passwd")
        {
            QMessageBox::information(NULL,"Error","支付失败,密码错误",QMessageBox::Yes);
            this->close();
        }
    }

}

void afford::release_bank()
{

}

void afford::on_ConfirmBtn_clicked()
{
    QString passwd=ui->input->text();
    QString all_in="0 "+bank_type+" "+card_id+" "+passwd+" "+QString::number(total,'f',2);;
    socket->write(all_in.toUtf8());
    socket->flush();
}


void afford::on_CancelBtn_clicked()
{
    this->close();
}
