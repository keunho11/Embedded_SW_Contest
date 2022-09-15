#include "page_1.h"
#include "ui_page_1.h"
#include "dht11_lx3.h"
#include <QPixmap>
#include <iostream>
#include <sstream>

dht11_lx3 dht111; //use dht11_lx3 to define a variable dht11

page_1::page_1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page_1)
{
    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timer1()));
    timer->start(1000);

    QTimer *timer2 = new QTimer(this);
    connect(timer2,SIGNAL(timeout()),this,SLOT(timer2()));
    timer2->start(5000);

    QLabel *label = new QLabel(this);
    label->setPixmap(QPixmap("humidity_icon.png"));
    label->setScaledContents(true);

    ui->label_date_time->setAlignment(Qt::AlignCenter);

    pSocketClient = new SocketClient(this);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(slotSocketSendData(QString)));
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(slotConnectToServer(bool)));


    setStyleSheet("QPushButton{ border: none; outline: none; padding: 0px}");
    //connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(slotPushButton()));

    QPixmap pix1(":/new/prefix1/temperature_icon.png");
    ui->label->setPixmap(pix1);
    ui->label->setScaledContents(true);
    ui->pTemp->setStyleSheet("color:#FF0000");
    ui->label_6->setStyleSheet("color:#FF0000");

    QPixmap pix2(":/new/prefix1/humi2.png");
    ui->label_2->setPixmap(pix2);
    ui->label_2->setScaledContents(true);
    ui->pHumi->setStyleSheet("color:#0000FF");
    ui->label_8->setStyleSheet("color:#0000FF");
    //ui->label_2->setPixmap(pix2.scaled(50,50, Qt::KeepAspectRatio));

}

page_1::~page_1()
{
    delete ui;
}

void page_1::slotConnectToServer(bool checked)
{
    bool bOk;
    if(checked)
    {
        pSocketClient->slotConnectToServer(bOk);
        emit pushButtonSig(1);
    }
    //else{
    //    pSocketClient->slotClosedByServer();
    //}
}

void page_1::timer1()
{
    QDateTime datetime = QDateTime::currentDateTime();
    QString text = datetime.toString("yyyy.MM.dd hh:mm:ss");

    ui->label_date_time->setText(text);
}

void page_1::timer2()
{
    dht111.dht11_read_data();
    //QString temp = dht111.buffer[2].toString;
    //printf("%s\n",&temp);
    //QString humi = QString::fromStdString(dht111->buffer[0]);
    ui->label_6->setText(QString::number(dht111.buffer[2])+"'C");
    ui->label_8->setText(QString::number(dht111.buffer[0])+"%");
}

void page_1::slotSocketSendData(QString strData)
{
    bool bOk;
    if(ui->pushButton->isChecked())
    {
        pSocketClient->slotSocketSendData(strData);
//        if(strData.indexOf("OFF") != -1)
//        {
//            pSocketClient->slotClosedByServer();
//        }
//        if(strData.indexOf("ON") != -1)
//        {
//            pSocketClient->slotConnectToServer(bOk);
//        }
    }
    else
        QMessageBox::information(this,"socket","서버 연결 확인");

}
