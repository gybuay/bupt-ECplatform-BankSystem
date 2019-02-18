#ifndef USER_H
#define USER_H
#include <QString>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <vector>
#include "user_list.h"
class User
{
public:
    User();
    User(QString a,QString b,QString c);
    bool userInsert();
    QString getId(){return id;}
    void pushList(user_list tmp){list.push_back(tmp);}
    void popList(){list.pop_back();}
    unsigned int sizeList(){return list.size();}
    user_list backList(){return list.back();}
    user_list selectList(int i){return list[i];}
    void clearList(){list.clear();return;}
    bool userLogin();

private:
    QString id;
    QString passwd;
    QString mobile;
    std::vector<user_list> list;//每个用户的购物车
};

#endif // USER_H
