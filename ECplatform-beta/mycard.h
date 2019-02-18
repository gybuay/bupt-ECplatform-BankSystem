#ifndef MYCARD_H
#define MYCARD_H

#include <QDialog>
#include "user.h"
#include "bind_card.h"
#include "afford.h"
namespace Ui {
class mycard;
}

class mycard : public QDialog
{
    Q_OBJECT

public:
    explicit mycard(QWidget *parent = 0);
    ~mycard();
    void getUser(User);
    void Init();
    void setTotal(double);
    void changeAfford(bool);
private slots:
    void on_CancelBtn_clicked();

    void on_pushButton_clicked();

    void on_BindBtn_clicked();


    void receive_card(QString,QString);

    void on_RemoveBtn_clicked();

private:
    Ui::mycard *ui;
    std::vector<QString> bank_type;
    std::vector<QString> card_id;
    User login_user;
    double total;
};

#endif // MYCARD_H
