#include "mainwindow.h"
#include <QApplication>
#include "bank.h"
#include <QMessageBox>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Bank b;
    b.Registered();
    return a.exec();
}
