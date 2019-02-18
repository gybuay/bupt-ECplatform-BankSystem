#include "bind_card.h"
#include "ui_bind_card.h"
#include <QMessageBox>
bind_card::bind_card(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::bind_card)
{
    ui->setupUi(this);
    setFixedSize(400,300);
    ui->passwd->setEchoMode(QLineEdit::Password);
    ui->comboBox->addItem("中国工商银行");
    ui->comboBox->addItem("中国建设银行");
    ui->comboBox->addItem("中国银行");
    ui->comboBox->addItem("交通银行");
    ui->comboBox->addItem("中国农业银行");
    ui->comboBox->addItem("招商银行");
    ui->comboBox->addItem("邮政储蓄银行");
    ui->comboBox->addItem("光大银行");
    ui->comboBox->addItem("民生银行");
    this->setWindowTitle("绑定银行卡");
    socket=new QTcpSocket();
    connect(socket,&QTcpSocket::readyRead,this,&bind_card::read_bank);
    connect(socket,&QTcpSocket::disconnected,this,&bind_card::release_bank);
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

bind_card::~bind_card()
{
    delete this->socket;
    delete ui;
}

void bind_card::read_bank()
{
    QString buffer;
    buffer = socket->readAll();
    qDebug()<<"in bind"<<buffer;
    if(!buffer.isEmpty())
    {
        if(buffer=="false in bind")
        {
            QMessageBox::information(NULL,"Error","输入银行卡信息错误，请重新输入",QMessageBox::Yes);
            return;
        }
        else if(buffer=="true in bind")
        {
            emit send_card(ui->card_id->text(),ui->comboBox->currentText());
        }
    }
    this->accept();
}

void bind_card::release_bank()
{

}

void bind_card::on_ConfirmBtn_clicked()
{
    QString card_id=ui->card_id->text();
    QString id=ui->id->text();
    QString name=ui->name->text();
    QString passwd=ui->passwd->text();
    QString bank_type=ui->comboBox->currentText();
    if(card_id=="" || id=="" || name=="" || passwd=="" || bank_type=="")
    {
        QMessageBox::information(NULL,"Error","请输入完整信息",QMessageBox::Yes);
        return;
    }
    QString all_in="1 "+card_id+" "+id+" "+name+" "+passwd+" "+bank_type;
    socket->write(all_in.toUtf8());
    socket->flush();
}

void bind_card::on_CancelBtn_clicked()
{
    this->close();
}
