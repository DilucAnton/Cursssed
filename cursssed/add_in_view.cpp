#include "add_in_view.h"
#include "ui_add_in_view.h"


#include <QRegularExpression>
#include <QDate>

add_in_view::add_in_view(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::add_in_view)
{
    ui->setupUi(this);


    this->ui->dateEdit->setDate(QDate::currentDate());
}

add_in_view::~add_in_view()
{
    delete ui;
}

void add_in_view::on_pushButton_clicked()
{
    static QRegularExpression regEx("^[а-яА-ЯёЁ]+([- ]?[-а-яА-ЯёЁ]+)*$");

    QSqlQuery query;




    QString title = ui->lineEdit->text();
    QString genre = ui->comboBox_2->currentText();
    int rel_year = ui->spinBox->value();
    QString director = ui->lineEdit_2->text();
    QString firstName = ui->lineEdit_3->text();
    QString lastName = ui->lineEdit_4->text();
    QString phone = ui->lineEdit_5->text();
    int amount = ui->spinBox_2->value();
    int price = ui->spinBox_3->value();
    QString paystat = ui->comboBox->currentText();


    if (!regEx.match(title).hasMatch()) {
            QMessageBox::warning(this, "Ошибка", "Поле 'Фильм' должно содержать только кирилицу ");
            return;
        }

    if (!regEx.match(firstName).hasMatch()) {
            QMessageBox::warning(this, "Ошибка", "Поле 'Имя' должно содержать только кирилицу ");
            return;
        }
    if (!regEx.match(lastName).hasMatch()) {
            QMessageBox::warning(this, "Ошибка", "Поле 'Фамилия' должно содержать только кирилицу ");
            return;
        }

    query.prepare("INSERT INTO MovieRentalsView (\"Название фильма\", \"Жанр\", \"Год выпуска\", \"Режиссер\", \"Цена билета\", \"Имя пользователя\", \"Фамилия пользователя\", \"Телефон\", \"Дата возврата\",\"Цена аренды\",\"Статус оплаты\")                  "
                  "VALUES (:title, :genre, :rel_year,:director,:price, :firstName, :lastName, :phone, :retDate, :amount, :payStat);");

    query.bindValue(":title", title);
    query.bindValue(":genre", genre);
    query.bindValue(":rel_year", rel_year);
    query.bindValue(":director", director);
    query.bindValue(":price", price);
    query.bindValue(":firstName", firstName);
    query.bindValue(":lastName", lastName);
    query.bindValue(":phone", phone);
    query.bindValue(":amount", amount);
    query.bindValue(":retDate", ui->dateEdit->date());
    query.bindValue(":payStat", paystat);


    msg = new QMessageBox();
    if (query.exec()) {
        // запрос выполнен успешно
        msg->setText("Успешно добавлено");
        this->close();


    } else {
        // произошла ошибка выполнения запроса
        msg->setText("Ошибка: " + query.lastError().text());
    }
    msg->show();

    emit refresh();
}

