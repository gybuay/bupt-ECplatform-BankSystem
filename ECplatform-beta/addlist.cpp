#include "addlist.h"
#include "ui_addlist.h"

addlist::addlist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addlist)
{
    ui->setupUi(this);
    ui->cnt->setPlaceholderText("非负整数");
    this->setWindowTitle("添加购物车");
    setFixedSize(355,105);
}

addlist::~addlist()
{
    delete ui;
}

void addlist::setBook(book tmp)
{
    Book=tmp;
}

void addlist::setFood(food tmp)
{
    Food=tmp;
}

void addlist::setClothes(clothes tmp)
{
    Clothes=tmp;
}

void addlist::setPage(int x)
{
    page=x;
}

void addlist::setUser(User tmp)
{
    user=tmp;
}

void addlist::on_ConfirmBtn_clicked()
{
    QString scnt=ui->cnt->text();
    if(scnt=="")
    {
        QMessageBox::information(NULL,"Error","请输入要添加至购物车的商品的数量",QMessageBox::Yes);
        return;
    }
    for(int i=0;i<scnt.length();i++)
        if(!(scnt[i]<='9' && scnt[i]>='0'))
        {
            QMessageBox::information(NULL,"Error","请输入正整数数量",QMessageBox::Yes);
            return;
        }
    int cnt=scnt.toInt();
    if(cnt==0)
    {
        QMessageBox::information(NULL,"Error","请输入正整数数量",QMessageBox::Yes);
        return;
    }

    shoplist sl;
    if(page == 0)
    {
        if(cnt>Book.getRemain())
        {
            QMessageBox::information(NULL,"Error","您输入的数量超过剩余量",QMessageBox::Yes);
            return;
        }
        list.setName(Book.getName());
        list.setType("图书");
        list.setDesc(Book.getDesc());
        list.setPrice(Book.getPrice());
        list.setCount(cnt);
        list.setTotal(Book.getPrice()*cnt);
        sl.setUser(user);
        bool check=sl.addIn(list,Book.getRemain());
        if(check)
            accept();
    }
    else if(page == 1)
    {
        if(cnt>Food.getRemain())
        {
            QMessageBox::information(NULL,"Error","您输入的数量超过剩余量",QMessageBox::Yes);
            return;
        }
        list.setName(Food.getName());
        list.setType("食品");
        list.setDesc(Food.getDesc());
        list.setPrice(Food.getPrice());
        list.setCount(cnt);
        list.setTotal(Food.getPrice()*cnt);
        sl.setUser(user);
        bool check=sl.addIn(list,Food.getRemain());
        if(check)
            accept();
    }
    else if(page == 2)
    {
        if(cnt>Clothes.getRemain())
        {
            QMessageBox::information(NULL,"Error","您输入的数量超过剩余量",QMessageBox::Yes);
            return;
        }
        list.setName(Clothes.getName());
        list.setType("服装");
        list.setDesc(Clothes.getDesc());
        list.setPrice(Clothes.getPrice());
        list.setCount(cnt);
        list.setTotal(Clothes.getPrice()*cnt);
        sl.setUser(user);
        bool check=sl.addIn(list,Clothes.getRemain());
        if(check)
            accept();
    }
}

void addlist::on_CancelBtn_clicked()
{
    this->close();
}
