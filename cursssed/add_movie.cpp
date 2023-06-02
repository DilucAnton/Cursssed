#include "add_movie.h"
#include "ui_add_movie.h"
#include <QRegularExpression>
add_movie::add_movie(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::add_movie)
{
    ui->setupUi(this);

}

add_movie::~add_movie()
{
    delete ui;
}

void add_movie::on_pushButton_clicked()
{
    QSqlQuery query;

    static QRegularExpression regEx("^[а-яА-ЯёЁ]+([- ]?[-а-яА-ЯёЁ]+)*$");

    QString title = ui->lineEdit->text();
    QString director = ui->lineEdit_3->text();

    if (!regEx.match(title).hasMatch()) {
            QMessageBox::warning(this, "Ошибка", "Поле 'Фильм' должно содержать только кирилицу ");
            return;
        }
    if (!regEx.match(director).hasMatch()) {
            QMessageBox::warning(this, "Ошибка", "Поле 'Режиссер' должно содержать только кирилицу ");
            return;
        }

    query.prepare("select public.add_movie(:Title, :Genre, :Director, :Release_year, :Price)");
    query.bindValue(":Title", title);
    query.bindValue(":Genre", ui->comboBox->currentText());
    query.bindValue(":Director", director);
    query.bindValue(":Release_year", ui->spinBox->value());
    query.bindValue(":Price", ui->spinBox_2->value());

    msg = new QMessageBox();
    if (query.exec()) {
        // запрос выполнен успешно
        msg->setText("Фильм успешно добавлен");
        this->close();


    } else {
        // произошла ошибка выполнения запроса
        msg->setText("Ошибка: " + query.lastError().text());
    }
    msg->show();


    emit refresh();
}

