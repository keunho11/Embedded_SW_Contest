#include "page_4.h"
#include "ui_page_4.h"

page_4::page_4(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page_4)
{
    ui->setupUi(this);
    sqlDb = QSqlDatabase::addDatabase("QSQLITE");
    sqlDb.setDatabaseName("temp.db");
    if(sqlDb.open()){
        qDebug() << "success db connection";
    }
    else{
        qDebug() << "fail db connection";
    }

    QString query = "CREATE TABLE temp_db ("
                     "id integer primary key,"
                     "date DATETIME,"
                     "onoff VARCHAR(10))";
    QSqlQuery qry;
    if(qry.exec(query)){
        qDebug() << "already db exist";
    }
    else{
        qDebug() << "create table";
    }

    OnoffCheck = "OFF";

    connect(ui->pPBSearch, SIGNAL(clicked()), this, SLOT(slotSearchDb()));
    connect(ui->pPBDelete, SIGNAL(clicked()), this, SLOT(slotDeleteDb()));

}

page_4::~page_4()
{
    delete ui;
}

void page_4::slotPage4RecvData(QString recvData)
{
      qDebug()  << "db_recv : " << recvData;
      QDateTime dateTime= QDateTime::currentDateTime();
      QString query;
      QSqlQuery qry;

      if(OnoffCheck != recvData)
      {

      query = "insert into temp_db(date, onoff) values('" +
                dateTime.toString("yy/MM/dd hh:mm:ss") +
                "','" + recvData + "')";

      if(qry.exec(query)){
          qDebug() << "query ok!" ;
      }
      else{
          qDebug() << qry.lastError().text();
      }

      OnoffCheck = recvData;
      }
}

void page_4::slotSearchDb()
{
    QString query;
    QSqlQuery qry;
    QDateTime dateTimeFrom = ui->pdateTimeEditFrom->dateTime();
    QString strFrom = dateTimeFrom.toString("yy/MM/dd hh:mm:ss");
    QDateTime dateTimeTo = ui->pdateTimeEditTo->dateTime();
    QString strTo = dateTimeTo.toString("yy/MM/dd hh:mm:ss");

    int rowCount = 0;
    ui->pTableWidget->clearContents(); // clear monitor
    query = "select * from temp_db where date > '" + strFrom +"' AND date < '" + strTo + "'" ;
    if(qry.exec(query)){
        while(qry.next()){
            rowCount++;
            ui->pTableWidget->setRowCount(rowCount);
            QTableWidgetItem *id = new QTableWidgetItem();
            QTableWidgetItem *date = new QTableWidgetItem();
            QTableWidgetItem *onoff = new QTableWidgetItem();

            id->setText(qry.value("id").toString());
            date->setText(qry.value("date").toString());
            onoff->setText(qry.value("onoff").toString());

            ui->pTableWidget->setItem(rowCount-1, 0,id);
            ui->pTableWidget->setItem(rowCount-1, 1,date);
            ui->pTableWidget->setItem(rowCount-1, 2,onoff);
        }
    }
    else{

    }
}

void page_4::slotDeleteDb()
{
    QString query;
    QSqlQuery qry;
    QDateTime dateTimeFrom = ui->pdateTimeEditFrom->dateTime();
    QString strFrom = dateTimeFrom.toString("yy/MM/dd hh:mm:ss");
    QDateTime dateTimeTo = ui->pdateTimeEditTo->dateTime();
    QString strTo = dateTimeTo.toString("yy/MM/dd hh:mm:ss");

    query = "delete from temp_db where date > '" + strFrom + "' AND date < '" + strTo + "'";
    if(qry.exec(query)){


    }

}

void page_4::on_pBack_page4_clicked()
{
    emit pushButtonSig(1);
}
