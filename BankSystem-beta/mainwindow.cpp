#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bank.h"
#include "bank_login.h"
#include "bank_register.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("呱呱银行信息系统");
    ui->passwd_input->setEchoMode(QLineEdit::Password);
    Init();
    server = new QTcpServer();
    connect(server, &QTcpServer::newConnection, this, &MainWindow::server_New_Connect);
    server->listen(QHostAddress::Any, 26666);
    socket = NULL;
    qDebug()<<server->isListening();
}

MainWindow::~MainWindow()
{
    server->close();
    server->deleteLater();
    delete ui;
}

void MainWindow::on_LoginBtn_clicked()
{
    Bank t;
    bank_login bl;
    connect(this,SIGNAL(send_update(double)),&bl,SLOT(receive_update(double)));
    QString _card_id,_passwd,_bank_type;
    _card_id=ui->card_id_input->text();
    _passwd=ui->passwd_input->text();
    _bank_type=ui->bank_input->currentText();
    t.LoginBank(_card_id,_passwd,_bank_type);
    bool flag=t.CheckLogin();
    if(flag)
    {
        bl.GetInfo(t);
        QMessageBox::information(NULL,"success","登录成功",QMessageBox::Yes);
        if(bl.exec() == QDialog::Accepted)
        {
            ui->card_id_input->setText("");
            ui->passwd_input->setText("");

        }
    }
    else
        QMessageBox::information(NULL,"error","账号密码错误",QMessageBox::Yes);
}

void MainWindow::on_RegisterBtn_clicked()
{
    bank_register br;
    br.exec();
}

void MainWindow::Init()
{
    ui->card_id_input->setText("");
    ui->passwd_input->setText("");
    QSqlDatabase data;
    data=QSqlDatabase::addDatabase("QSQLITE");
    data.setDatabaseName("BankData.db");
    if(!data.open())
    {
        QMessageBox::information(NULL,"Error","打开银行数据库BankData.db失败",QMessageBox::Yes);
    }
    else
    {
        QSqlQuery query(data);
        QString select_info="SELECT name from BankName";
        if(!query.exec(select_info))
            ;//QMessageBox::information(NULL,"Error","Failed to execute select",QMessageBox::Yes);
        else
        {
            while(query.next())
            {
                QString tmp_name;
                tmp_name=query.value(0).toString();
                ui->bank_input->addItem(tmp_name);
            }
        }
    }
}

void MainWindow::server_New_Connect()
{
    //获取客户端连接
    socket = server->nextPendingConnection();
    QObject::connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socket_Read_Data);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socket_Disconnected);
}

void MainWindow::socket_Read_Data()
{
    QString buffer;
    buffer = socket->readAll();
    if(!buffer.isEmpty())
    {
        QSqlDatabase data;
        data=QSqlDatabase::addDatabase("QSQLITE","socket");
        data.setDatabaseName("BankData.db");
        QSqlQuery query(data);

        if(!data.open())
        {
            QMessageBox::information(NULL,"Error","打开银行数据库BankData.db失败",QMessageBox::Yes);
            return;
        }
        if(buffer[0]=="1")
        {
            QString card_id=buffer.section(' ',1,1);
            QString id=buffer.section(' ',2,2);
            QString name=buffer.section(' ',3,3);
            QString passwd=buffer.section(' ',4,4);
            QString bank_type=buffer.section(' ',5,5);

            QString select_info="SELECT * FROM BankSystem WHERE card_id=="+card_id;
            if(!query.exec(select_info))
            {
                //QMessageBox::information(NULL,"Error","输入银行卡信息错误，请重新输入",QMessageBox::Yes);
                qDebug()<<"bank false in bind";
                socket->write("false in bind");
                socket->flush();
                return;
            }
            else
            {
                while(query.next())
                {
                    QString tmp_passwd=query.value(1).toString();
                    QString tmp_bank=query.value(2).toString();
                    QString tmp_name=query.value(3).toString();
                    QString tmp_id=query.value(4).toString();
                    if(tmp_bank==bank_type && tmp_id==id && tmp_name==name && tmp_passwd==passwd)
                    {
                        socket->write("true in bind");
                        socket->flush();
                        qDebug()<<"bank true in bind";
                        return;
                    }
                }
                socket->write("false in bind");
                socket->flush();
                qDebug()<<"bank true in bind";
                return;
                //QMessageBox::information(NULL,"Success","绑定成功！",QMessageBox::Yes);         
            }
        }
        else if(buffer[0]=="0")
        {
            QString bank_type=buffer.section(' ',1,1);
            QString card_id=buffer.section(' ',2,2);
            QString passwd=buffer.section(' ',3,3);
            double total=buffer.section(' ',4,4).toDouble();

            QString select_info="SELECT * FROM BankSystem WHERE card_id=="+card_id;
            if(!query.exec(select_info))
            {
                //QMessageBox::information(NULL,"Error","输入银行卡信息错误，请重新输入",QMessageBox::Yes);
                qDebug()<<"bank false in afford";
                socket->write("false in afford");
                socket->flush();
                return;
            }
            else
            {
                while(query.next())
                {
                    QString tmp_passwd=query.value(1).toString();
                    QString tmp_bank=query.value(2).toString();
                    double tmp_remain=query.value(5).toDouble();
                    if(tmp_bank==bank_type && tmp_passwd==passwd)
                    {
                        if(tmp_remain >= total)
                        {
                            QString update_info="UPDATE BankSystem set remain=:remain WHERE card_id=="\
                                    +card_id+" AND bank_name==\""+bank_type+"\"";
                            query.prepare(update_info);
                            query.bindValue(":remain",tmp_remain-total);
                            if(!query.exec())
                            {
                                socket->write("false in afford");
                                socket->flush();
                                qDebug()<<"bank false in afford";
                                return;
                            }
                            socket->write("true in afford");
                            socket->flush();
                            emit send_update(tmp_remain-total);
                            qDebug()<<"bank true in afford";
                            return;
                        }
                        socket->write("false in afford remain");
                        socket->flush();
                        qDebug()<<"bank false in afford remain";
                        return;
                    }
                }
                socket->write("false in afford passwd");
                socket->flush();
                qDebug()<<"bank false in afford remain";
            }
        }
    }
}

void MainWindow::socket_Disconnected()
{
    qDebug() << "Disconnected!";
}
