#ifndef QUERIES_H
#define QUERIES_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>


#include "querymodel.h"

namespace Ui {
class queries;
}

class queries : public QWidget
{
    Q_OBJECT

public:
    explicit queries(QWidget *parent = nullptr);
    ~queries();
    QMessageBox *msg;

private slots:
    void on_c1Button_clicked();

    void on_c2Button_clicked();

    void on_c3Button_clicked();

    void on_anyButton_clicked();

    void on_allButton_clicked();

    void on_caseButton_clicked();

    void on_manytablesButton_clicked();

    void on_cor1Button_clicked();

    void on_cor2Button_clicked();

    void on_cor3Button_clicked();

    void on_viewButton_clicked();

private:
    Ui::queries *ui;

    QSqlQueryModel *qmodel;
    QueryModel *qmod;
};

#endif // QUERIES_H
