#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QHostAddress>
#include "bank.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void send_update(double);

private slots:

    void on_LoginBtn_clicked();
    void on_RegisterBtn_clicked();
    void server_New_Connect();
    void socket_Read_Data();
    void socket_Disconnected();
private:
    Ui::MainWindow *ui;
    void Init();
    QTcpServer *server;
    QTcpSocket *socket;
    bool toupdate;
};

#endif // MAINWINDOW_H
