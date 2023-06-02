#include "upd_payments.h"
#include "ui_upd_payments.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>


upd_payments::upd_payments(int paymentID, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::upd_payments)
{

    ui->setupUi(this);

    this->paymentID = paymentID;


    QSqlQueryModel* rentalsmodel = qmod->rentalsSelect();
    this->ui->comboBox->setModel(rentalsmodel);
    this->ui->comboBox->setModelColumn(1);

    fillData();
}

upd_payments::~upd_payments()
{
    delete ui;
}

void upd_payments::fillData()
{

    QSqlQuery query;
    query.prepare("SELECT rental_id, amount, date FROM payments WHERE id = :ID");
    query.bindValue(":ID", this->paymentID);
    if (query.exec())
    {
        query.next();

        int rentalID = query.value(0).toInt();
        int amount = query.value(1).toInt();

        ui->spinBox->setValue(amount);
        ui->dateEdit->setDate(query.value(2).toDate());


        QComboBox* rentalBox = ui->comboBox;

        for (int i = 0; i < rentalBox->count(); i++)
        {
            int rowID = rentalBox->model()->data(rentalBox->model()->index(i,0)).toInt();
            if (rowID == rentalID)
            {
                rentalBox->setCurrentIndex(i);
                break;
            }
        }

    }



}

void upd_payments::on_pushButton_clicked()
{
    QAbstractItemModel* rentalModel = this->ui->comboBox->model();

    int rentalID = rentalModel->data(rentalModel->index(ui->comboBox->currentIndex(),0)).toInt();

    int amount = ui->spinBox->value();
    QSqlQuery query;

    QDate thisDate = ui->dateEdit->date();

    query.prepare("SELECT update_payments(:ID, :rental_id, :amount, :date)");
        query.bindValue(":ID", this->paymentID);
        query.bindValue(":rental_id", rentalID);
        query.bindValue(":amount", amount);
        query.bindValue(":date",thisDate);


    if (query.exec())
    {
        this->close();
    } else {
        QMessageBox msg;
        msg.setText("Не получилось изменить данные "+ query.lastError().text());
        msg.setIcon(QMessageBox::Critical);
        msg.exec();
        qDebug() << query.lastError().text();
    }
    emit refresh();
}

