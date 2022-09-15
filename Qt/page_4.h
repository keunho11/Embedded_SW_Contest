#ifndef PAGE_4_H
#define PAGE_4_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>

namespace Ui {
class page_4;
}

class page_4 : public QWidget
{
    Q_OBJECT

public:
    explicit page_4(QWidget *parent = nullptr);
    ~page_4();

private:
    Ui::page_4 *ui;
    QSqlDatabase sqlDb;
    QString OnoffCheck;

signals:
    void pushButtonSig(int);

private slots:
    void on_pBack_page4_clicked();
    void slotPage4RecvData(QString);
    void slotSearchDb();
    void slotDeleteDb();
};

#endif // PAGE_4_H
