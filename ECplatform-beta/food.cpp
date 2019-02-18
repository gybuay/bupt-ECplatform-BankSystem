#include "food.h"

food::food()
{

}

QString food::getMaintain()
{
    return maintain;
}

double food::getDiscount()
{
    return discount;
}

void food::setMaintain(QString x)
{
    maintain=x;
}

void food::setDiscount(double x)
{
    discount=x;
}

double food::getPrice()
{
    price=product::getPrice()*discount;
    return price;
}


