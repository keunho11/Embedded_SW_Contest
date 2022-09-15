#ifndef ALARM_H
#define ALARM_H

#include <QObject>
#include <QString>
#include <QTimer>

class Alarm : public QObject
{
    Q_OBJECT
public:
    virtual ~Alarm(){};
    static Alarm & GetInstance()
    {
        static Alarm instance;
        return instance;
    }
    void Start();
    void Stop();

private:
    explicit Alarm(QObject *parent = 0);
    
signals:
    
public slots:
    //void Resume();
};

#endif // ALARM_H
