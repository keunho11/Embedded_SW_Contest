#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QTime>
#include <QDate>
#include <QDateTime>
#include <QDebug>
#include <QTimer>

#include <page_1.h>
#include <page_2.h>
#include <page_3.h>
#include <page_4.h>
#include "timer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private slots:
    void slotChangeIndex(int);

private:
    Ui::MainWidget *ui;
    page_1* ppage_1;
    page_2* ppage_2;
    page_3* ppage_3;
    page_4* ppage_4;
    Timer* ptimer;
};
#endif // MAINWIDGET_H
