#ifndef BOOK_H
#define BOOK_H
#include "product.h"

class book:public product
{
public:
    book();
    void setAuthor(QString x);
    QString getAuthor();
    void setDiscount(double x);
    double getDiscount();
    double getPrice();
private:
    double discount;
    double price;
    QString author;
};

#endif // BOOK_H
