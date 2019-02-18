#ifndef BANK_DEPOSIT_H
#define BANK_DEPOSIT_H

#include <QDialog>
#include "bank.h"
namespace Ui {
class bank_deposit;
}

class bank_deposit : public QDialog
{
    Q_OBJECT

public:
    explicit bank_deposit(QWidget *parent = 0);
    ~bank_deposit();
    void GetInfo(Bank t);
    Bank PassInfo(){return login_user;}
private slots:
    void on_ConfirmBtn_clicked();

    void on_CancelBtn_clicked();

private:
    Ui::bank_deposit *ui;
    Bank login_user;
};

#endif // BANK_DEPOSIT_H
