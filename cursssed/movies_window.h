#ifndef MOVIES_WINDOW_H
#define MOVIES_WINDOW_H

#include <QWidget>
#include <QMenu>
#include <QSqlQueryModel>
#include <QMessageBox>
#include "add_movie.h"
#include "ui_add_movie.h"

namespace Ui {
class movies_window;
}

class movies_window : public QWidget
{
    Q_OBJECT

public:
    int globint;


    explicit movies_window(QWidget *parent = nullptr);
    ~movies_window();
    QMessageBox *msg;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

    void CustomMenuReq(QPoint);

    void deleteFragment();


private:
    Ui::movies_window *ui;
    QSqlQueryModel *qmodel;
    add_movie *addmv;
};

#endif // MOVIES_WINDOW_H
