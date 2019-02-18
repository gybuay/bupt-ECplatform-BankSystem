#include "plat_register.h"
#include "ui_plat_register.h"
#include "user.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>

plat_register::plat_register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::plat_register)
{
    ui->setupUi(this);
    setFixedSize(318,316);
    this->setWindowTitle("账户注册");
    ui->id->setFocus();
    ui->id->setPlaceholderText("至少6位字符");
    ui->passwd0->setPlaceholderText("至少6位字符");
    ui->passwd1->setPlaceholderText("至少6位字符");
    ui->mobile->setPlaceholderText("11位数字");
    ui->ConfirmBtn->setFocus();
    ui->ConfirmBtn->setDefault(true);
}

plat_register::~plat_register()
{
    delete ui;
}

void plat_register::on_ConfirmBtn_clicked()
{
    QString _id=ui->id->text();
    QString _passwd0=ui->passwd0->text();
    QString _passwd1=ui->passwd1->text();
    QString _mobile=ui->mobile->text();
    if(_id=="" || _passwd0=="" || _passwd1=="" || _mobile=="")
    {
        QMessageBox::information(NULL,"Error","任何一项不能为空",QMessageBox::Yes);
        return;
    }
    if(_passwd0 != _passwd1)
    {
        QMessageBox::information(NULL,"Error","两次输入密码不一致",QMessageBox::Yes);
        return;
    }
    for(int i=0;i<_mobile.size();i++)
        if(_mobile[i]>'9' || _mobile[i]<'0' || _mobile.size()!=11)
        {
            QMessageBox::information(NULL,"Error","手机号应为11位数字",QMessageBox::Yes);
            return;
        }
    if(_id.length()<6)
    {
        QMessageBox::information(NULL,"Error","账户至少6位字符",QMessageBox::Yes);
        return;
    }
    if(_passwd0.length()<6)
    {
        QMessageBox::information(NULL,"Error","密码至少6位字符",QMessageBox::Yes);
        return;
    }
    User tmp(_id,_passwd0,_mobile);
    if(tmp.userInsert())
        this->accept();
}

void plat_register::on_CancelBtn_clicked()
{
    this->close();
}
