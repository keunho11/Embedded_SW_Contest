#ifndef PAGE_1_H
#define PAGE_1_H

#include <QWidget>
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include "socketclient.h"
#include "dht11_lx3.h"

namespace Ui {
class page_1;
}

class page_1 : public QWidget
{
    Q_OBJECT

public:
    explicit page_1(QWidget *parent = nullptr);
    ~page_1();

public slots:
    void timer1();
    void timer2();

private:
    Ui::page_1 *ui;
    SocketClient *pSocketClient;

private slots:
    void slotConnectToServer(bool);
    //void on_pushButton_clicked(bool);
    void slotSocketSendData(QString);


signals:
    void pushButtonSig(int);
};

#endif // PAGE_1_H
