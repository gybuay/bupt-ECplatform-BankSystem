#ifndef AFFORD_H
#define AFFORD_H

#include <QDialog>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>
namespace Ui {
class afford;
}

class afford : public QDialog
{
    Q_OBJECT

public:
    explicit afford(QWidget *parent = 0);
    ~afford();
    void setInfo(QString,QString,double);
private slots:
    void on_ConfirmBtn_clicked();
    void read_bank();
    void release_bank();
    void on_CancelBtn_clicked();

private:
    Ui::afford *ui;
    QTcpSocket *socket;
    double total;
    QString card_id;
    QString bank_type;
};

#endif // AFFORD_H
