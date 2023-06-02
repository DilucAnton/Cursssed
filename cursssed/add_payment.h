#ifndef ADD_PAYMENT_H
#define ADD_PAYMENT_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

#include "querymodel.h"

namespace Ui {
class add_payment;
}

class add_payment : public QWidget
{
    Q_OBJECT

public:
    explicit add_payment(QWidget *parent = nullptr);
    ~add_payment();
    QMessageBox *msg;
signals:
    void refresh();

private slots:
    void on_pushButton_clicked();

private:
    Ui::add_payment *ui;
    QueryModel *qmod;
    QSqlQuery *query;
};

#endif // ADD_PAYMENT_H
