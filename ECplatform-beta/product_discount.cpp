#include "product_discount.h"
#include "ui_product_discount.h"
#include <QMessageBox>
product_discount::product_discount(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::product_discount)
{
    ui->setupUi(this);
    setFixedSize(264,119);
    this->setWindowTitle("促销折扣设置");
    ui->comboBox->addItem("图书");
    ui->comboBox->addItem("食品");
    ui->comboBox->addItem("服装");
}

product_discount::~product_discount()
{
    delete ui;
}

void product_discount::on_ConfirmBtn_clicked()
{
    double discount=ui->input->text().toDouble();

    if(discount>0 && discount<=1)
    {
         emit send_discount(discount,ui->comboBox->currentText());
         accept();
    }
    else
    {
        QMessageBox::information(NULL,"Error","请输入0~1之间的系数，不能为0",QMessageBox::Yes);
    }
}

void product_discount::on_CancelBtn_clicked()
{
    this->close();
}
