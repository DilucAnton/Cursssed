#include "querymodel.h"

#include <QSqlQueryModel>
#include <QString>
#include <QSqlQuery>
#include <QSqlError>


QueryModel::QueryModel()
{

}



QSqlQueryModel* QueryModel::userSelectAllList() {
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT id, CONCAT(first_name, ' ', LEFT(last_name, 1), '. ')  FROM users ORDER BY id");
    return model;
}

QSqlQueryModel* QueryModel::moviesSelectAllList() {
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT id, CONCAT(title, ' - ', genre)  FROM movies ORDER BY id");
    return model;
}



QSqlQueryModel* QueryModel::rentalsSelectAllList() {
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT r.id, CONCAT(u.first_name, ' ', LEFT(u.last_name, 1), '., Фильм: ', m.title, ', ','Дата аренды: ', r.checkout_date) "
    "FROM Rentals r "
    "INNER JOIN Users u ON r.user_id = u.id "
    "INNER JOIN Movies m ON r.movie_id = m.id "
    "LEFT JOIN Payments p ON r.id = p.rental_id "
    "WHERE p.id IS NULL "
    "ORDER BY r.id");
    return model;
}

QSqlQueryModel* QueryModel::rentalsSelect()
{
    QSqlQueryModel * model = new QSqlQueryModel;
    model->setQuery("SELECT r.id, CONCAT(u.first_name, ' ', LEFT(u.last_name, 1), '., Фильм: ', m.title, ', ','Дата аренды: ', r.checkout_date) FROM Rentals r INNER JOIN Users u ON r.user_id = u.id INNER JOIN Movies m ON r.movie_id = m.id; ");
    return model;
}


QSqlQueryModel* QueryModel::query_c1() {
    QSqlQueryModel *model = new QSqlQueryModel;

    model->setQuery("SELECT m.title AS \"Название Фильма\", (SELECT COUNT(*) FROM Rentals r WHERE r.movie_id = m.id) AS \"Количество Аренд\" FROM Movies m;");
    return model;
}

QSqlQueryModel* QueryModel::query_c2(QString value) {
    QSqlQueryModel *model = new QSqlQueryModel;

    QString Value = value;
    model->setQuery("SELECT m.title AS \"Название Фильма\", r.rental_count as \"Количество Аренд\" FROM Movies m JOIN (SELECT movie_id, COUNT(*) AS rental_count FROM Rentals WHERE movie_id IN (SELECT id FROM Movies WHERE genre = '"+ Value +"') GROUP BY movie_id) r ON m.id = r.movie_id;");
    return model;
}


QSqlQueryModel* QueryModel::query_c3() {
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT CONCAT(Users.first_name, ' ', Users.last_name) AS Пользователь, Users.phone AS Телефон, Movies.title AS Фильм FROM Rentals JOIN Users ON Rentals.user_id = Users.id JOIN Movies ON Rentals.movie_id = Movies.id WHERE Rentals.total_cost > (SELECT AVG(amount) FROM Payments);");
    return model;
}

QSqlQueryModel* QueryModel::query_Any(QString value) {
    QSqlQueryModel *model = new QSqlQueryModel;
    QString Value = value;
    model->setQuery("SELECT first_name AS \"Имя пользователя\", last_name AS \"Фамилия пользователя\", phone AS \"Телефон\" FROM Users WHERE id = ANY (SELECT user_id FROM Rentals WHERE movie_id IN (SELECT id FROM Movies WHERE genre = '" + Value + "'))");
return model;
}

QSqlQueryModel* QueryModel::query_All(QString value) {
    QSqlQueryModel *model = new QSqlQueryModel;
    QString boolean;
    if(value=="Оплачена")
    {
        boolean = "true";
    }
    else
        boolean = "false";
    model->setQuery("SELECT first_name AS \"Имя пользователя\", last_name AS \"Фамилия пользователя\", phone AS \"Телефон\" FROM Users u WHERE EXISTS (SELECT 1 FROM Rentals WHERE rentals.user_id = u.id) AND '" + boolean + "' = ALL (SELECT rentals.payment_status FROM Rentals WHERE rentals.user_id = u.id)");
    return model;
}

QSqlQueryModel* QueryModel::query_CASE() {
    QSqlQueryModel *model = new QSqlQueryModel;
    QString query = "SELECT Movies.title AS \"Название фильма\", Movies.genre AS \"Жанр фильма\", COUNT(Rentals.id) AS \"Количество аренд\", CASE WHEN COUNT(Payments.id) > 0 THEN SUM(Payments.amount) ELSE 0.00 END AS \"Итоговая сумма платежей\" FROM Movies LEFT JOIN Rentals ON Movies.id = Rentals.movie_id LEFT JOIN Payments ON Rentals.id = Payments.rental_id GROUP BY Movies.title, Movies.genre;";

    model->setQuery(query);
    return model;
}

QSqlQueryModel* QueryModel::query_ManyTables(){
    QSqlQueryModel *model = new QSqlQueryModel;

    model->setQuery("SELECT CONCAT(Users.first_name,' ',Users.last_name) AS \"Пользователь\", COUNT(Rentals.id) AS \"Количество оплаченых аренд\", SUM(Payments.amount) AS \"Сумма оплат всех аренд\" FROM Rentals JOIN Users ON Rentals.user_id = Users.id JOIN Payments ON Rentals.id = Payments.rental_id GROUP BY Rentals.user_id, Users.first_name, Users.last_name HAVING COUNT(Rentals.id) >= 2 AND SUM(Payments.amount) > 30000;");
    return model;

}



QSqlQueryModel* QueryModel::query_corr_1()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT CONCAT(first_name, ' ', last_name) AS Пользователь FROM Users WHERE EXISTS (SELECT 1 FROM Rentals LEFT JOIN Payments ON Rentals.id = Payments.rental_id WHERE Rentals.user_id = Users.id) AND NOT EXISTS (SELECT 1 FROM Rentals LEFT JOIN Payments ON Rentals.id = Payments.rental_id WHERE Rentals.user_id = Users.id AND Payments.id IS NULL);");
    return model;
}

QSqlQueryModel* QueryModel::query_corr_2()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT title AS Фильм FROM Movies WHERE EXISTS (SELECT 1 FROM Rentals INNER JOIN Payments ON Rentals.id = Payments.rental_id WHERE Rentals.movie_id = Movies.id GROUP BY Rentals.movie_id HAVING AVG(Payments.amount) > (SELECT AVG(amount) FROM Payments));");
    return model;
}
QSqlQueryModel* QueryModel::query_corr_3()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT m.title AS Фильм, CONCAT(u.first_name, ' ', u.last_name) AS Пользователь FROM Movies m INNER JOIN Rentals r ON r.movie_id = m.id INNER JOIN Users u ON u.id = r.user_id WHERE EXISTS (SELECT 1 FROM Payments p WHERE p.rental_id = r.id AND p.amount >= (SELECT AVG(amount) FROM Payments)) ORDER BY m.title;");
    return model;
}







