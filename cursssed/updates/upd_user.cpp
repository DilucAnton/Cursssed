#include "upd_user.h"
#include "ui_upd_user.h"

#include <QRegularExpression>

#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

upd_user::upd_user(int userID, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::upd_user)
{
    ui->setupUi(this);
    this->userID = userID;

    fillData();
}

upd_user::~upd_user()
{
    delete ui;
}

void upd_user::fillData()
{
    QSqlQuery query;
        query.prepare("SELECT id, first_name, last_name, phone FROM users WHERE id = :ID");
        query.bindValue(":ID", this->userID);
        if (query.exec())
        {
            query.next();
            ui->lineEdit->setText(query.value(1).toString());
            ui->lineEdit_2->setText(query.value(2).toString());
            ui->lineEdit_3->setText(query.value(3).toString());
        }

}
void upd_user::on_pushButton_clicked()
{
    QString first_name = this->ui->lineEdit->text();
    QString last_name = this->ui->lineEdit_2->text();
    QString phone = this->ui->lineEdit_3->text();
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


    query.prepare("SELECT update_users(:ID, :first_name, :last_name, :phone)");
    query.bindValue(":ID", this->userID);
    query.bindValue(":first_name", first_name);
    query.bindValue(":last_name", last_name);
    query.bindValue(":phone", phone);



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

