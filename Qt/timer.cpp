#include "timer.h"
#include "socketclient.h"
#include <QTimer>
#include <QMessageBox>
#include <QtDebug>

Timer::Timer(QObject *parent,QTime *Collection) :
    QObject(parent)
{
    this->_Schedule=Collection;
    //pSocketClient = new SocketClient(this);
}

void Timer::StartTimer(QTime *MainAlarm,bool* alramcheck)
{
    this->_Schedule=MainAlarm;
    this->_Check = alramcheck;
    QTimer *timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(AlarmCheck()));
    timer->start(1000);
}

void Timer::AlarmCheck()
{
    //Compare saved times with now time
    QTime RightNow=QTime::currentTime();//We're in now, now...
    if(*_Check == true)
    {
        if(this->_Schedule->hour()==RightNow.hour()
           && _Schedule->minute()==RightNow.minute()
           && _Schedule->second()==RightNow.second())
        {
            qDebug() << "Alram ON!!";
            QString strTotalData;
            strTotalData = "[ALLMSG]OFF";
            qDebug() << strTotalData;
            emit sigSocketRecv(strTotalData);
            emit page4RecvData("OFF");

            QMessageBox msgBox;
            msgBox.setText("Alarm ON");
            msgBox.exec();
        }
    }


}

