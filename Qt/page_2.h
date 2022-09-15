#ifndef PAGE_2_H
#define PAGE_2_H

#include <QWidget>
#include <QTime>
#include <QDebug>
#include <QMessageBox>
#include "socketclient.h"

namespace Ui {
class page_2;
}

class page_2 : public QWidget
{
    Q_OBJECT

public:
    explicit page_2(QWidget *parent = nullptr);
    ~page_2();


private:
    Ui::page_2 *ui;
    SocketClient *pSocketClient;
    //QTcpSocket *socket;
private slots:

    void on_pToggle_toggled(bool);

    void on_pushButton_clicked();

    void on_pDB_clicked();

    void on_pBack_page2_clicked();

signals:
    void pushButtonSig(int);
    void sigSocketRecv(QString);
    void page1RecvData(QString);
    void page4RecvData1(QString);
    void sigSocketSendData(QString);

};

#endif // PAGE_2_H
