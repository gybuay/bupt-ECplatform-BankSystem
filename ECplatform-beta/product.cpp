#include "product.h"

product::product()
{

}

QString product::getName()
{
    return name;
}

QString product::getDesc()
{
    return desc;
}

int product::getRemain()
{
    return remain;
}

double product::getPrice()
{
    return price;
}

void product::setName(QString x)
{
    name=x;
}

void product::setDesc(QString x)
{
    desc=x;
}

void product::setPrice(double x)
{
    price=x;
}

void product::setRemain(int x)
{
    remain=x;
}
