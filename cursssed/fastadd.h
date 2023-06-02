#ifndef FASTADD_H
#define FASTADD_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>


namespace Ui {
class fastAdd;
}

class fastAdd : public QWidget
{
    Q_OBJECT

public:
    explicit fastAdd(QWidget *parent = nullptr);
    ~fastAdd();

    QMessageBox *msg;

signals:
    void refresh();
private slots:
    void on_pushButton_clicked();

private:
    Ui::fastAdd *ui;

    QSqlQuery *query;
};

#endif // FASTADD_H
