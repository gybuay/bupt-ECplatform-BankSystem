#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "state.h"
#include "user.h"
#include "book.h"
#include "food.h"
#include "clothes.h"
#include <QMainWindow>
#include <vector>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    int state;
    User login_user;
    double fulfill;
    double tominus;
    double discount;
    std::vector<book> Book;
    std::vector<food> Food;
    std::vector<clothes> Clothes;
    std::vector<book> searchBook;
    std::vector<food> searchFood;
    std::vector<clothes> searchClothes;
    void InitBook();
    void InitFood();
    void InitClothes();
    void changeWindow();
signals:
    clicked();
private slots:
    void slot_append();
    void tominus_receive(double,double);
    void discount_receive(double,QString);
    void result_receive(user_list);
    void receive_user(User);
    void on_login_clicked();
    void on_registered_clicked();
    void on_shoplist_clicked();
    void on_setDiscount_clicked();
    void on_discount2_clicked();
    void on_addlist_clicked();
    void on_admin_login_clicked();
    void on_LogoutBtn_clicked();
    void on_BindBtn_clicked();
    void on_SearchBtn_clicked();
};

#endif // MAINWINDOW_H
