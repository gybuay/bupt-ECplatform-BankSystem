#ifndef BANK_H
#define BANK_H
#include<QString>
class Bank
{
private:
    QString bank_type;//银行种类
    QString user;//持卡人姓名
    QString id;//持卡人身份证
    QString card_id;//卡号
    QString passwd;//密码
    double remain;//余额
public:
    Bank();
    QString GetUser(){return user;}
    QString GetBank(){return bank_type;}
    double GetRemain(){return remain;}
    void Update(QString tmp,int n);
    void LoginBank(QString _card_id,QString _passwd,QString _bank_type);//登录
    bool RegisterBank(QString _user,QString _id,QString _card_id,QString _passwd,QString _bank);//注册
    bool CheckLogin();
    bool CheckRegister();
    void Registered();
};

#endif // BANK_H
