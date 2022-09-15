#include "mainwidget.h"
#include "ui_mainwidget.h"

# include  "dht11_lx3.h"
# include  <wiringPi.h>
dht11_lx3 dht11 ; //use dht11_lx3 to define a variable dht11

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    ppage_1 = new page_1(ui->page_1);
    ui->page_1->setLayout(ppage_1->layout());
    ppage_2 = new page_2(ui->page_2);
    ui->page_2->setLayout(ppage_2->layout());
    ppage_3 = new page_3(ui->page_3);
    ui->page_3->setLayout(ppage_3->layout());
    ppage_4 = new page_4(ui->page_4);
    ui->page_4->setLayout(ppage_4->layout());

    //ptimer = new Timer;
    dht11.pininit ( ) ; //initialize pin

    QPixmap bg(":/new/prefix1/background.png");

    QPalette p(palette());
    p.setBrush(QPalette::Background, bg);

    setAutoFillBackground(true);
    setPalette(p);


    connect(ppage_1,SIGNAL(pushButtonSig(int)),this,SLOT(slotChangeIndex(int)));
    connect(ppage_2,SIGNAL(pushButtonSig(int)),this,SLOT(slotChangeIndex(int)));
    connect(ppage_3,SIGNAL(pushButtonSig(int)),this,SLOT(slotChangeIndex(int)));
    connect(ppage_4,SIGNAL(pushButtonSig(int)),this,SLOT(slotChangeIndex(int)));

    connect(ppage_2,SIGNAL(sigSocketRecv(QString)),ppage_1,SLOT(slotSocketSendData(QString)));
    connect(ppage_3->TimeKeeper,SIGNAL(sigSocketRecv(QString)),ppage_1,SLOT(slotSocketSendData(QString)));
    connect(ppage_2,SIGNAL(page4RecvData1(QString)),ppage_4,SLOT(slotPage4RecvData(QString)));
    connect(ppage_3->TimeKeeper,SIGNAL(page4RecvData(QString)),ppage_4,SLOT(slotPage4RecvData(QString)));

}

void MainWidget::slotChangeIndex(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}

MainWidget::~MainWidget()
{
    delete ui;
}


