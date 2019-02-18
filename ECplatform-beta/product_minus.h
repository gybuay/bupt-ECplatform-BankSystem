#ifndef PRODUCT_MINUS_H
#define PRODUCT_MINUS_H

#include <QDialog>

namespace Ui {
class product_minus;
}

class product_minus : public QDialog
{
    Q_OBJECT

public:
    explicit product_minus(QWidget *parent = 0);
    ~product_minus();

signals:
    void send_tominus(double,double);

private slots:
    void on_ConfirmBtn_clicked();

    void on_CancelBtn_clicked();

private:
    Ui::product_minus *ui;
};

#endif // PRODUCT_MINUS_H
