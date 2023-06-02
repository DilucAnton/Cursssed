#ifndef ADD_RENTAL_H
#define ADD_RENTAL_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

#include "querymodel.h"


namespace Ui {
class add_rental;
}

class add_rental : public QWidget
{
    Q_OBJECT

public:
    explicit add_rental(QWidget *parent = nullptr);
    ~add_rental();
    QMessageBox *msg;
signals:
    void refresh();

private slots:
    void on_pushButton_clicked();

private:
    Ui::add_rental *ui;
    QueryModel *qmod;
};

#endif // ADD_RENTAL_H
