#ifndef PRODUCT_H
#define PRODUCT_H
#include <QString>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
class product
{
public:
    product();
    QString getName();
    QString getDesc();
    int getRemain();
    void setName(QString x);
    void setDesc(QString x);
    void setRemain(int x);
    void setPrice(double x);
    virtual double getPrice();
private:
    QString name;
    QString desc;
    double price;
    int remain;
};

#endif // PRODUCT_H
