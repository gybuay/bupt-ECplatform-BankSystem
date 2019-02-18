#ifndef BANK_REGISTER_H
#define BANK_REGISTER_H

#include <QDialog>

namespace Ui {
class bank_register;
}

class bank_register : public QDialog
{
    Q_OBJECT

public:
    explicit bank_register(QWidget *parent = 0);
    ~bank_register();
    bool Check();

private slots:
    void on_ConfirmBtn_clicked();

    void on_CancelBtn_clicked();

private:
    Ui::bank_register *ui;
};

#endif // BANK_REGISTER_H
