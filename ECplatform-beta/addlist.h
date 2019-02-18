#ifndef ADDLIST_H
#define ADDLIST_H

#include <QDialog>
#include "book.h"
#include "food.h"
#include "clothes.h"
#include "user.h"
#include "shoplist.h"
namespace Ui {
class addlist;
}

class addlist : public QDialog
{
    Q_OBJECT

public:
    explicit addlist(QWidget *parent = 0);
    ~addlist();
    void setBook(book tmp);
    void setFood(food tmp);
    void setClothes(clothes tmp);
    void setPage(int x);
    void setUser(User tmp);
private slots:
    void on_ConfirmBtn_clicked();

    void on_CancelBtn_clicked();

private:
    Ui::addlist *ui;
    book Book;
    food Food;
    clothes Clothes;
    user_list list;
    User user;
    int page;
};

#endif // ADDLIST_H
