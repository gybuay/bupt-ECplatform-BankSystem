#ifndef BANK_DRAW_H
#define BANK_DRAW_H

#include <QDialog>
#include "bank.h"

namespace Ui {
class bank_draw;
}

class bank_draw : public QDialog
{
    Q_OBJECT

public:
    explicit bank_draw(QWidget *parent = 0);
    ~bank_draw();
    void GetInfo(Bank t);
    Bank PassInfo(){return login_user;}

private slots:
    void on_ConfirmBtn_clicked();

    void on_CancelBtn_clicked();

private:
    Ui::bank_draw *ui;
    Bank login_user;
};

#endif // BANK_DRAW_H
