#ifndef FOOD_H
#define FOOD_H
#include "product.h"

class food:public product
{
public:
    food();
    void setMaintain(QString x);
    QString getMaintain();
    void setDiscount(double x);
    double getDiscount();
    double getPrice();
private:
    double discount;
    double price;
    QString maintain;
};

#endif // FOOD_H
