#ifndef BIND_CARD_H
#define BIND_CARD_H

#include <QDialog>
#include <QTcpSocket>
#include <QHostAddress>

namespace Ui {
class bind_card;
}

class bind_card : public QDialog
{
    Q_OBJECT

public:
    explicit bind_card(QWidget *parent = 0);
    ~bind_card();
signals:
    void send_card(QString,QString);
private slots:
    void on_ConfirmBtn_clicked();

    void on_CancelBtn_clicked();

    void read_bank();
    void release_bank();

private:
    Ui::bind_card *ui;
    QTcpSocket *socket;
};

#endif // BIND_CARD_H
