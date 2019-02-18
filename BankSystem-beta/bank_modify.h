#ifndef BANK_MODIFY_H
#define BANK_MODIFY_H

#include <QDialog>
#include "bank.h"
namespace Ui {
class bank_modify;
}

class bank_modify : public QDialog
{
    Q_OBJECT

public:
    explicit bank_modify(QWidget *parent = 0);
    ~bank_modify();
    void GetInfo(Bank t);
    Bank PassInfo(){return login_user;}

private slots:
    void on_CancelBtn_clicked();

    void on_ConfirmBtn_clicked();

private:
    Ui::bank_modify *ui;
    Bank login_user;
};

#endif // BANK_MODIFY_H
