#include "user_list.h"

user_list::user_list()
{

}

void user_list::setName(QString x)
{
    name=x;
}

void user_list::setType(QString x)
{
    type=x;
}

void user_list::setDesc(QString x)
{
    describe=x;
}

void user_list::setPrice(double x)
{
    price=x;
}

void user_list::setCount(int x)
{
    count=x;
}

void user_list::setTotal(double x)
{
    total=x;
}

QString user_list::getName()
{
    return name;
}

QString user_list::getType()
{
    return type;
}

QString user_list::getDesc()
{
    return describe;
}

double user_list::getPrice()
{
    return price;
}

int user_list::getCount()
{
    return count;
}

double user_list::getTotal()
{
    return total;
}
