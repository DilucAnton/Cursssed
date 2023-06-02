#include "add_user.h"
#include "ui_add_user.h"
#include <QRegularExpression>

add_user::add_user(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::add_user)
{
    ui->setupUi(this);
}

add_user::~add_user()
{
    delete ui;
}

void add_user::on_pushButton_clicked()
{
    QSqlQuery query;
    QString frsname = ui->lineEdit->text();
    QString lastname = ui->lineEdit_2->text();
    QString Phone = ui->lineEdit_3->text();

    static QRegularExpression regEx("^[а-яА-ЯёЁ]+([- ]?[-а-яА-ЯёЁ]+)*$");
    if (!regEx.match(frsname).hasMatch()) {
            QMessageBox::warning(this, "Ошибка", "Поле 'Имя' должно содержать только кирилицу ");
            return;
        }
    if (!regEx.match(lastname).hasMatch()) {
            QMessageBox::warning(this, "Ошибка", "Поле 'Фамилия' должно содержать только кирилицу ");
            return;
        }

    static QRegularExpression regExpPhone("\\d+");
    if (!regExpPhone.match(Phone).hasMatch()) {
            QMessageBox::warning(this, "Ошибка", "Поле 'Телефон' должно содержать только цифры ");
            return;
        }

    query.prepare("select public.add_user(:FirstName, :LastName, :Phone)");
    query.bindValue(":FirstName", ui->lineEdit->text());
    query.bindValue(":LastName", ui->lineEdit_2->text());
    query.bindValue(":Phone", ui->lineEdit_3->text());



    msg = new QMessageBox();
    if (query.exec()) {
        // запрос выполнен успешно
        msg->setText("Пользователь успешно добавлен");
        this->close();


    } else {
        // произошла ошибка выполнения запроса
        msg->setText("Ошибка: " + query.lastError().text());
    }
    msg->show();

    emit refresh();
}

