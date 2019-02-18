#ifndef BANK_LOGIN_H
#define BANK_LOGIN_H

#include <QDialog>
#include "bank.h"
namespace Ui {
class bank_login;
}

class bank_login : public QDialog
{
    Q_OBJECT

public:
    explicit bank_login(QWidget *parent = 0);
    void GetInfo(Bank t);
    ~bank_login();

public slots:
    void receive_update(double);

private slots:
    void on_DepositBtn_clicked();

    void on_DrawBtn_clicked();

    void on_LogoutBtn_clicked();

    void on_ModifyBtn_clicked();


private:
    Ui::bank_login *ui;
    Bank login_user;
};

#endif // BANK_LOGIN_H
