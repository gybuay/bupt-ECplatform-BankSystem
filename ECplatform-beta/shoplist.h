#ifndef SHOPLIST_H
#define SHOPLIST_H

#include <QDialog>
#include <QString>
#include "user.h"
#include "mycard.h"
namespace Ui {
class shoplist;
}

class shoplist : public QDialog
{
    Q_OBJECT

public:
    explicit shoplist(QWidget *parent = 0);
    ~shoplist();
    void setUser(User tmp);
    void Init();
    void add();
    void remove();
    void setTominus(double,double);
    void setDiscnt(double);
    bool addIn(user_list tmp,int remain);

signals:
    void send_result(user_list);
private slots:
    void on_RemoveBtn_clicked();

    void on_ConfirmBtn_clicked();

    void on_CancelBtn_clicked();

private:
    Ui::shoplist *ui;
    User login_user;
    user_list list;
    double fulfill;
    double tominus;
    double discount;
    double topay;
};

#endif // SHOPLIST_H
