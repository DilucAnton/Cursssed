#include "add_payment.h"
#include "ui_add_payment.h"


#include <QMessageBox>
#include <QComboBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>


add_payment::add_payment(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::add_payment)
{
    ui->setupUi(this);


    QSqlQueryModel* rentalsmodel = qmod->rentalsSelectAllList();
    this->ui->comboBox->setModel(rentalsmodel);
    this->ui->comboBox->setModelColumn(1);
}

add_payment::~add_payment()
{
    delete ui;
}

void add_payment::on_pushButton_clicked()
{

    QAbstractItemModel* rentalsmodel = this->ui->comboBox->model();

    int rentalId = rentalsmodel->data(rentalsmodel->index(ui->comboBox->currentIndex(),0)).toInt();


    QDate currentDate = QDate::currentDate();

    QSqlQuery query;

    int amount = ui->spinBox->value();



    query.prepare("SELECT add_payment(:rentalId, :amount, :date)");
        query.bindValue(":rentalId", rentalId);
        query.bindValue(":amount", amount);
        query.bindValue(":date",currentDate);


     msg = new QMessageBox();
        if (query.exec()) {
            // запрос выполнен успешно
            msg->setText("Платёж успешно добавлен");
            this->close();


        } else {
            // произошла ошибка выполнения запроса
            msg->setText("Ошибка: " + query.lastError().text());
        }
        msg->show();

    emit refresh();
}

