#include "clothes.h"

clothes::clothes()
{

}

QString clothes::getAttr()
{
    return attribute;
}

double clothes::getDiscount()
{
    return discount;
}

void clothes::setAttr(QString x)
{
    attribute=x;
}

void clothes::setDiscount(double x)
{
    discount=x;
}

double clothes::getPrice()
{
    price=product::getPrice()*discount;
    return price;
}
