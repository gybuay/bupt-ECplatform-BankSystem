#include "plat_login.h"
#include "ui_plat_login.h"

plat_login::plat_login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::plat_login)
{
    ui->setupUi(this);
    this->setWindowTitle("登录");
    setFixedSize(400,300);
    ui->passwd->setEchoMode(QLineEdit::Password);
}

plat_login::~plat_login()
{
    delete ui;
}

void plat_login::on_ConfirmBtn_clicked()
{
    QString id=ui->id->text();
    QString passwd=ui->passwd->text();
    if(id=="" || passwd=="")
    {
        QMessageBox::information(NULL,"Error","请输入账号密码",QMessageBox::Yes);
        return;
    }
    User tmp(id,passwd,"");
    bool check=tmp.userLogin();
    if(check)
    {
        emit send_user(tmp);
        QMessageBox::information(NULL,"Success","登录成功",QMessageBox::Yes);
        accept();
    }
    else
    {
        QMessageBox::information(NULL,"Success","账号密码错误",QMessageBox::Yes);
    }
}

void plat_login::on_CancelBtn_clicked()
{
    this->close();
}
