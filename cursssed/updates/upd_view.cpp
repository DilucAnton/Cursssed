#include "upd_view.h"
#include "ui_upd_view.h"


#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

upd_view::upd_view(int rentalID, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::upd_view)
{
    ui->setupUi(this);


    this->rentalID = rentalID;

    fillData();
}

upd_view::~upd_view()
{
    delete ui;
}

void upd_view::fillData()
{
    QSqlQuery query;
    query.prepare("SELECT \"ID фильма\", \"ID пользователя\", \"Название фильма\", \"Жанр\", \"Год выпуска\",\"Режиссер\",\"Цена билета\", \"Имя пользователя\", \"Фамилия пользователя\", \"Телефон\",\"Дата возврата\", \"Цена аренды\" FROM MovieRentalsView WHERE \"ID аренды\" = :ID;");
    query.bindValue(":ID", this->rentalID);

    if (query.exec())
    {
        query.next();

        movieID = query.value(0).toInt();
        userID = query.value(1).toInt();

        ui->lineEdit->setText(query.value(2).toString());

        QString genre = query.value(3).toString();

        ui->spinBox->setValue(query.value(4).toInt());
        ui->lineEdit_2->setText(query.value(5).toString());

        ui->spinBox_2->setValue(query.value(6).toInt());
        ui->lineEdit_3->setText(query.value(7).toString());
        ui->lineEdit_4->setText(query.value(8).toString());
        ui->lineEdit_5->setText(query.value(9).toString());
        ui->dateEdit->setDate(query.value(10).toDate());
        ui->spinBox_3->setValue(query.value(11).toInt());




        QComboBox* genreBox = ui->comboBox;

        for (int i = 0; i< genreBox->count();i++)
        {
            QString inGenre = genreBox->model()->data(genreBox->model()->index(i,0)).toString();

            if (inGenre == genre)
            {
                genreBox->setCurrentIndex(i);
                break;
            }

        }
    }




}

void upd_view::on_pushButton_clicked()
{
    QString movie = this->ui->lineEdit->text();

    QAbstractItemModel* genreModel = this->ui->comboBox->model();
    QString genre = genreModel->data(genreModel->index(ui->comboBox->currentIndex(),0)).toString();

    int releaseYear = this->ui->spinBox->value();
    QString director = this->ui->lineEdit_2->text();
    int price = this->ui->spinBox_2->value();
    QString first_name = this->ui->lineEdit_3->text();
    QString last_name = this->ui->lineEdit_4->text();
    QString phone = this->ui->lineEdit_5->text();
    QDate returnDate = this->ui->dateEdit->date();
    int amount = this->ui->spinBox_3->value();


    QSqlQuery query;



    static QRegularExpression regEx("^[а-яА-ЯёЁ]+([- ]?[-а-яА-ЯёЁ]+)*$");
    static QRegularExpression regExpPhone("\\d+");

    if (!regExpPhone.match(phone).hasMatch()) {
            QMessageBox::warning(this, "Ошибка", "Поле 'Телефон' должно содержать только цифры ");
            return;
        }
    if (!regEx.match(first_name).hasMatch()) {
            QMessageBox::warning(this, "Ошибка", "Поле 'Телефон' должно содержать только кирилицу ");
            return;
        }
    if (!regEx.match(last_name).hasMatch()) {
            QMessageBox::warning(this, "Ошибка", "Поле 'Фамилия' должно содержать только кирилицу");
            return;
        }


    query.prepare("UPDATE MovieRentalsView SET "
                      "\"Название фильма\" = :movie, "
                      "\"Жанр\" = :genre, "
                      "\"Год выпуска\" = :releaseYear, "
                      "\"Режиссер\" = :director, "
                      "\"Цена билета\" = :price, "
                      "\"Имя пользователя\" = :firstName, "
                      "\"Фамилия пользователя\" = :lastName, "
                      "\"Телефон\" = :phone, "
                      "\"Дата возврата\" = :returnDate, "
                      "\"Цена аренды\" = :amount "
                      "WHERE \"ID аренды\" = :rentalID;");

    query.bindValue(":movie", movie);
    query.bindValue(":genre", genre);
    query.bindValue(":releaseYear", releaseYear);
    query.bindValue(":director", director);
    query.bindValue(":price", price);
    query.bindValue(":firstName", first_name);
    query.bindValue(":lastName", last_name);
    query.bindValue(":phone", phone);
    query.bindValue(":returnDate", returnDate);
    query.bindValue(":amount", amount);
    query.bindValue(":rentalID", rentalID);


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

