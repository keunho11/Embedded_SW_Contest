#include "alarm.h"

#include <QString>
#include <QDir>
#include <QDebug>

Alarm::Alarm(QObject *parent) :
    QObject(parent)
{
    //connect(this->_Pause,SIGNAL(timeout()),this,SLOT(Resume()));
}

void Alarm::Start()
{
    qDebug() << "Alarm Start";
}

void Alarm::Stop()
{
    qDebug() << "Alarm Stop";
}

/*
void Alarm::Resume()
{
    this->_Pause->stop();
}
*/

