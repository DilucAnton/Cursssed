#include "add_rental.h"
#include "ui_add_rental.h"


#include <QMessageBox>
#include <QComboBox>
#include <QSqlQuery>
#include <QSqlError>


add_rental::add_rental(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::add_rental)
{
    ui->setupUi(this);

    QSqlQueryModel* usersmodal = qmod->userSelectAllList();
    this->ui->comboBox->setModel(usersmodal);
    this->ui->comboBox->setModelColumn(1);

    QSqlQueryModel* moviemodal = qmod->moviesSelectAllList();
    this->ui->comboBox_2->setModel(moviemodal);
    this->ui->comboBox_2->setModelColumn(1);



    this->ui->dateEdit->setDate(QDate::currentDate());

}
add_rental::~add_rental()
{
    delete ui;
}





void add_rental::on_pushButton_clicked()
{
    QAbstractItemModel* usermodel = this->ui->comboBox->model();
    QAbstractItemModel* moveismodel = this->ui->comboBox_2->model();

    int userId = usermodel->data(usermodel->index(ui->comboBox->currentIndex(),0)).toInt();
    int moviesId = moveismodel->data(moveismodel->index(ui->comboBox_2->currentIndex(),0)).toInt();
    QString status = "Активна";

    QSqlQuery query;

    QDate currentDate = QDate::currentDate();
    QDate returnDate = ui->dateEdit->date();


    query.prepare("SELECT add_rental(:userId, :moviesId, :checkOutDate, :returnDate, :status)");
        query.bindValue(":userId", userId);
        query.bindValue(":moviesId", moviesId);
        query.bindValue(":checkOutDate",currentDate);
        query.bindValue(":returnDate", returnDate);
        query.bindValue(":status", status);


     msg = new QMessageBox();
        if (query.exec()) {
            // запрос выполнен успешно
            msg->setText("Аренда успешно добавлена");
            this->close();


        } else {
            // произошла ошибка выполнения запроса
            msg->setText("Ошибка: " + query.lastError().text());
        }
        msg->show();

    emit refresh();
}

