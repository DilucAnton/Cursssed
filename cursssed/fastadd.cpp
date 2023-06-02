#include "fastadd.h"
#include "ui_fastadd.h"

#include <QRegularExpression>
#include <QDate>

fastAdd::fastAdd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fastAdd)
{
    ui->setupUi(this);

    this->ui->dateEdit->setDate(QDate::currentDate());

}

fastAdd::~fastAdd()
{
    delete ui;
}



void fastAdd::on_pushButton_clicked()
{
    static QRegularExpression regEx("^[а-яА-ЯёЁ]+([- ]?[-а-яА-ЯёЁ]+)*$");

    QSqlQuery query;


    QDate currentDate = QDate::currentDate();


    QString title = ui->lineEdit->text();
    QString director = ui->lineEdit_2->text();

    QString firstName = ui->lineEdit_3->text();
    QString lastName = ui->lineEdit_4->text();
    QString phone = ui->lineEdit_5->text();
    int amount = ui->spinBox_3->value();

    if (!regEx.match(title).hasMatch()) {
            QMessageBox::warning(this, "Ошибка", "Поле 'Фильм' должно содержать только кирилицу ");
            return;
        }
    if (!regEx.match(director).hasMatch()) {
            QMessageBox::warning(this, "Ошибка", "Поле 'Режиссер' должно содержать только кирилицу ");
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

    query.prepare("select public.create_movie_rental(:Title, :Genre, :Director, :Release_year, :Price, :firstName, :lastName, :phone, :checkDate,:returnDate, :amount, :checkDate )");
    query.bindValue(":Title", title);
    query.bindValue(":Genre", ui->comboBox->currentText());
    query.bindValue(":Director", director);
    query.bindValue(":Release_year", ui->spinBox->value());
    query.bindValue(":Price", ui->spinBox_2->value());
    query.bindValue(":checkDate", currentDate);
    query.bindValue(":firstName", firstName);
    query.bindValue(":lastName", lastName);
    query.bindValue(":phone", phone);
    query.bindValue(":amount", amount);
    query.bindValue(":returnDate", ui->dateEdit->date());

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

