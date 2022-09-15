#ifndef TIMER_H
#define TIMER_H

#include <QObject>
#include <QDateTime>
#include <QList>
#include "alarm.h"
#include <QTimer>


class Timer : public QObject
{
    Q_OBJECT
public:
    void StartTimer(QTime*, bool*);
    explicit Timer(QObject *parent = 0,QTime* Collection=0);

private:
    QTime *_Schedule;
    bool* _Check;


signals:
    void sigSocketRecv(QString);
    //void sigSocketSendData(QString);
    void page4RecvData(QString);

private slots:
    void AlarmCheck();
};

#endif // TIMER_H
