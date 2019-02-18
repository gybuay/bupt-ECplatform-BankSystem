#ifndef PLAT_LOGIN_H
#define PLAT_LOGIN_H

#include <QDialog>
#include "user.h"
namespace Ui {
class plat_login;
}

class plat_login : public QDialog
{
    Q_OBJECT

public:
    explicit plat_login(QWidget *parent = 0);
    ~plat_login();

private slots:
    void on_ConfirmBtn_clicked();

    void on_CancelBtn_clicked();

private:
    Ui::plat_login *ui;

signals:
    void send_user(User);
};

#endif // PLAT_LOGIN_H
