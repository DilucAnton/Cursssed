#ifndef ADD_USER_H
#define ADD_USER_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

namespace Ui {
class add_user;
}

class add_user : public QWidget
{
    Q_OBJECT

public:
    explicit add_user(QWidget *parent = nullptr);
    ~add_user();
    QMessageBox *msg;
signals:
    void refresh();
private slots:
    void on_pushButton_clicked();

private:
    Ui::add_user *ui;
    QSqlQueryModel *qmodel;
    QSqlQuery *query;
};

#endif // ADD_USER_H
