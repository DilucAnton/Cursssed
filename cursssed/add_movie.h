#ifndef ADD_MOVIE_H
#define ADD_MOVIE_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>


namespace Ui {
class add_movie;
}

class add_movie : public QWidget
{
    Q_OBJECT

public:
    explicit add_movie(QWidget *parent = nullptr);
    ~add_movie();
    QMessageBox *msg;

private slots:
    void on_pushButton_clicked();

signals:
    void refresh();

private:
    Ui::add_movie *ui;
    QSqlQueryModel *qmodel;
    QSqlQuery *query;
};

#endif // ADD_MOVIE_H
