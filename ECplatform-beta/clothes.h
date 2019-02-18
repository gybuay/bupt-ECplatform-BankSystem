#ifndef CLOTHES_H
#define CLOTHES_H
#include "product.h"


class clothes:public product
{
public:
    clothes();
    void setAttr(QString x);
    QString getAttr();
    void setDiscount(double x);
    double getDiscount();
    double getPrice();
private:
    double discount;
    double price;
    QString attribute;
};

#endif // CLOTHES_H
