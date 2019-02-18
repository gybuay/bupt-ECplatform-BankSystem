#ifndef PLAT_REGISTER_H
#define PLAT_REGISTER_H

#include <QDialog>

namespace Ui {
class plat_register;
}

class plat_register : public QDialog
{
    Q_OBJECT

public:
    explicit plat_register(QWidget *parent = 0);
    ~plat_register();

private slots:
    void on_ConfirmBtn_clicked();

    void on_CancelBtn_clicked();

private:
    Ui::plat_register *ui;
};

#endif // PLAT_REGISTER_H
