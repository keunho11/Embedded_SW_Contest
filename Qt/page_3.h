#ifndef PAGE_3_H
#define PAGE_3_H

#include "timer.h"
#include <QWidget>
#include <QTimer>
#include <QTime>
#include <QDialogButtonBox>
#include <QListWidgetItem>

namespace Ui {
class page_3;
}

class page_3 : public QWidget
{
    Q_OBJECT

public:
    explicit page_3(QWidget *parent = nullptr);
    ~page_3();

    QTime *displayTime = new QTime;
    QTimer *timer = new QTimer;
    bool buttonStart = true;
    Timer *TimeKeeper;
private:
    Ui::page_3 *ui;


    Alarm *CurAlarm;
    QTime *AlarmTime;
    bool* _isAlram;

    bool edit_on;
    bool dial_on;

    void SetupClock();
    void ShowActiveAlarm(bool, bool);

private slots:
    void on_pBack_page3_clicked();
    void UpdateClock();
    void SetTime();
    void ClearTime();

signals:
    void sigSocketSendData(QString);
    void pushButtonSig(int);

};

#endif // PAGE_3_H
