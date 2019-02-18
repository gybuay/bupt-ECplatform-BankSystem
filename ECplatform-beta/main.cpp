#include "mainwindow.h"
#include <QApplication>
#include "book.h"
#include "product.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
