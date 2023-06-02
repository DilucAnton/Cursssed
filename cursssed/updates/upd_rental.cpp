#include "upd_rental.h"
#include "ui_upd_rental.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

upd_rental::upd_rental(int rentalID, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::upd_rental)
{
    ui->setupUi(this);

    this->rentalID = rentalID;



    QSqlQueryModel* usersmodal = qmod->userSelectAllList();
    this->ui->comboBox->setModel(usersmodal);
    this->ui->comboBox->setModelColumn(1);

    QSqlQueryModel* moviemodal = qmod->moviesSelectAllList();
    this->ui->comboBox_2->setModel(moviemodal);
    this->ui->comboBox_2->setModelColumn(1);

    fillData();

}

upd_rental::~upd_rental()
{
    delete ui;
}

void upd_rental::fillData()
{
    QSqlQuery query;
    query.prepare("SELECT user_id, movie_id, checkout_date, return_date, status  FROM rentals WHERE id = :ID");
    query.bindValue(":ID", this->rentalID);
    if (query.exec())
    {
        query.next();

        int userID = query.value(0).toInt();
        int movieID = query.value(1).toInt();

        QString status = query.value(4).toString();

        ui->dateEdit->setDate(query.value(2).toDate());
        ui->dateEdit_2->setDate(query.value(3).toDate());


        QComboBox* userBox = ui->comboBox;

        for (int i = 0; i < userBox->count(); i++)
        {
            int rowID = userBox->model()->data(userBox->model()->index(i,0)).toInt();
            if (rowID == userID)
            {
                userBox->setCurrentIndex(i);
                break;
            }
        }

        QComboBox* movieBox = ui->comboBox_2;

        for (int i = 0; i< movieBox->count(); i++)
        {
            int rowID = movieBox->model()->data(movieBox->model()->index(i,0)).toInt();
            if (rowID == movieID)
            {
                movieBox->setCurrentIndex(i);
                break;
            }
        }

        QComboBox* statusBox = ui->comboBox_3;

        for (int i = 0; i< statusBox->count();i++)
        {
            QString inStatus = statusBox->model()->data(statusBox->model()->index(i,0)).toString();

            if (inStatus == status)
            {
                statusBox->setCurrentIndex(i);
                break;
            }

        }
    }



}

void upd_rental::on_pushButton_clicked()
{


    QAbstractItemModel* usermodel = this->ui->comboBox->model();
    QAbstractItemModel* moveismodel = this->ui->comboBox_2->model();
    QAbstractItemModel* statusmodel = this->ui->comboBox_3->model();

    QString status = statusmodel->data(statusmodel->index(ui->comboBox_3->currentIndex(),0)).toString();
    int userId = usermodel->data(usermodel->index(ui->comboBox->currentIndex(),0)).toInt();
    int moviesId = moveismodel->data(moveismodel->index(ui->comboBox_2->currentIndex(),0)).toInt();

    QSqlQuery query;

    QDate givenDate = ui->dateEdit->date();
    QDate returnDate = ui-> dateEdit_2->date();

    query.prepare("SELECT update_rentals(:ID, :userId, :moviesId, :checkOutDate, :returnDate, :status)");
        query.bindValue(":ID", this->rentalID);
        query.bindValue(":userId", userId);
        query.bindValue(":moviesId", moviesId);
        query.bindValue(":checkOutDate",givenDate);
        query.bindValue(":returnDate", returnDate);
        query.bindValue(":status", status);


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

