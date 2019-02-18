#ifndef USER_LIST_H
#define USER_LIST_H

#include <QString>
class user_list
{
public:
    user_list();
    void setName(QString x);
    void setType(QString x);
    void setDesc(QString x);
    void setPrice(double x);
    void setCount(int x);
    void setTotal(double x);
    QString getName();
    QString getType();
    QString getDesc();
    double getPrice();
    int getCount();
    double getTotal();
private:
    QString name;
    QString type;
    QString describe;
    double price;
    int count;
    double total;
};

#endif // USER_LIST_H
