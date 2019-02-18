#include "book.h"

book::book()
{
}

QString book::getAuthor()
{
    return author;
}

double book::getDiscount()
{
    return discount;
}

void book::setAuthor(QString x)
{
    author=x;
}

void book::setDiscount(double x)
{
    discount=x;
}

double book::getPrice()
{
    price=product::getPrice()*discount;
    return price;
}
