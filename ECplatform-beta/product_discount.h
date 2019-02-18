#ifndef PRODUCT_DISCOUNT_H
#define PRODUCT_DISCOUNT_H

#include <QDialog>

namespace Ui {
class product_discount;
}

class product_discount : public QDialog
{
    Q_OBJECT

public:
    explicit product_discount(QWidget *parent = 0);
    ~product_discount();

private slots:
    void on_ConfirmBtn_clicked();
    void on_CancelBtn_clicked();

signals:
    void send_discount(double,QString);
private:
    Ui::product_discount *ui;
};

#endif // PRODUCT_DISCOUNT_H
