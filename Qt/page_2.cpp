#include "page_2.h"
#include "ui_page_2.h"
#include <QPixmap>

page_2::page_2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page_2)
{
    ui->setupUi(this);
    //pSocketClient = new SocketClient(this);

    ui->on_off_label->setText("OFF");

    ui->on_off_label->setAlignment(Qt::AlignCenter);
    ui->Alarm_label->setAlignment(Qt::AlignCenter);
    ui->Data_label->setAlignment(Qt::AlignCenter);
    connect(ui->pToggle,SIGNAL(clicked(bool)),this,SLOT(on_pToggle_toggled(bool)));
    setStyleSheet("QPushButton{ border: none; outline: none; padding: 0px}");

}

page_2::~page_2()
{
    delete ui;
}

void page_2::on_pToggle_toggled(bool check)
{
    qDebug() << "check : "<<check;
    if(check)
    {
        //ui->pToggle->setChecked(true);
        QString strTotalData;
        strTotalData = "[ALLMSG]ON";
        //qDebug() << strTotalData;
        emit sigSocketRecv(strTotalData);

        ui->on_off_label->setText("ON");
        emit page4RecvData1("ON");
    }
    else{
        //ui->pToggle->setChecked(false);
        QString strTotalData;
        strTotalData = "[ALLMSG]OFF";
        //qDebug() << strTotalData;
        emit sigSocketRecv(strTotalData);

        ui->on_off_label->setText("OFF");
        emit page4RecvData1("OFF");
    }
}

void page_2::on_pushButton_clicked()
{
    emit pushButtonSig(2);
}

void page_2::on_pDB_clicked()
{
    emit pushButtonSig(3);
}

void page_2::on_pBack_page2_clicked()
{
    emit pushButtonSig(0);
}
