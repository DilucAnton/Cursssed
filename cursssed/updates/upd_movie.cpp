#include "upd_movie.h"
#include "ui_upd_movie.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

upd_movie::upd_movie(int movieID, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::upd_movie)
{
    ui->setupUi(this);

    this->movieID = movieID;

    fillData();
}

upd_movie::~upd_movie()
{
    delete ui;
}


void upd_movie::fillData()
{
    QSqlQuery query;
        query.prepare("SELECT id, title, director, release_year, price FROM movies WHERE id = :ID");
        query.bindValue(":ID", this->movieID);
        if (query.exec())
        {
            query.next();
            ui->lineEdit->setText(query.value(1).toString());
            ui->lineEdit_2->setText(query.value(2).toString());
            ui->spinBox->setValue(query.value(3).toInt());
            ui->spinBox_2->setValue(query.value(4).toInt());
        }

}



void upd_movie::on_pushButton_clicked()
{
    QString title = this->ui->lineEdit->text();
    QString director = this->ui->lineEdit_2->text();
    int release_year = this->ui->spinBox->value();
    int price = this->ui->spinBox_2->value();
    QString genre = this->ui->comboBox->currentText();
    QSqlQuery query;


    query.prepare("SELECT update_movies(:ID, :title, :genre, :director, :release_year, :price)");
    query.bindValue(":ID", this->movieID);
    query.bindValue(":title", title);
    query.bindValue(":genre", genre);
    query.bindValue(":director", director);
    query.bindValue(":release_year", release_year);
    query.bindValue(":price", price);

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

