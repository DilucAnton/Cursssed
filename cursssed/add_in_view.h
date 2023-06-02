#ifndef ADD_IN_VIEW_H
#define ADD_IN_VIEW_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

namespace Ui {
class add_in_view;
}

class add_in_view : public QWidget
{
    Q_OBJECT

public:
    explicit add_in_view(QWidget *parent = nullptr);
    ~add_in_view();
    QMessageBox *msg;


signals:
    void refresh();

private slots:
    void on_pushButton_clicked();

private:
    Ui::add_in_view *ui;

    QSqlQuery *query;
};

#endif // ADD_IN_VIEW_H
