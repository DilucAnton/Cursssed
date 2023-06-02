#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QWidget>
#include <QMenu>

#include <QSqlDatabase>
#include <QModelIndex>
#include <QAction>

#include <QTableView>

#include "add_movie.h"
#include "ui_add_movie.h"
#include "connection_dialog.h"
#include "ui_connection_dialog.h"
#include "add_user.h"
#include "ui_add_user.h"
#include "add_rental.h"
#include "ui_add_rental.h"
#include "add_payment.h"
#include "ui_add_payment.h"

#include "updates/upd_movie.h"
#include "ui_upd_movie.h"
#include "updates/upd_user.h"
#include "ui_upd_user.h"
#include "updates/upd_rental.h"
#include "ui_upd_rental.h"
#include "updates/upd_payments.h"
#include "ui_upd_payments.h"
#include "queries.h"
#include "ui_queries.h"
#include "fastadd.h"
#include "ui_fastadd.h"
#include "add_in_view.h"
#include "ui_add_in_view.h"
#include "updates/upd_view.h"
#include "ui_upd_view.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


enum TableViewVariant{
    movies,
    users,
    rentals,
    payments,
    view
};

enum UserRole {
    Administrator,
    BasicUser,
    Viewer
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QMessageBox *msg;
    int globint;

    QString username;



public slots:
    void slotForConnect(QString username);

    void setupButtons();

private slots:
    //void CustomMenuReq(QPoint);

    //void deleteFragment();

    void on_actionConnect_to_DB_triggered();

    void on_pushButton_clicked(); //movies

    void on_actionAdd_Movie_triggered();

    void movie_refresh(); // обновление окна по кнопке

    void users_refresh();

    void rentals_refresh();

    void payments_refresh();

    void view_refresh();


    void scalAndVecFunc();

    UserRole userRoles();

    //------------------------------------------





    void on_pushButton_2_clicked();

    void on_actionAdd_User_triggered();

    void on_table_movies_customContextMenuRequested(const QPoint &pos);




    void delAction(int selectedID, TableViewVariant selectedTable);

    void delActView(int RentID);

    void modAction(int selectedID, TableViewVariant selectedTable);

    void modActView(int RentID);



    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_table_users_customContextMenuRequested(const QPoint &pos);

    void on_actionAdd_Rental_triggered();

    void on_table_rentals_customContextMenuRequested(const QPoint &pos);

    void on_table_payments_customContextMenuRequested(const QPoint &pos);

    void on_actionAdd_Payment_triggered();

    void on_action_triggered();

    void on_fastAddButton_clicked();

    void on_pushButton_5_clicked();

    void on_table_view_customContextMenuRequested(const QPoint &pos);

    void on_actionAdd_InView_triggered();

private:
    Ui::MainWindow *ui;
    connection_dialog *con_dlg;
    QSqlQueryModel *qmodel;
    add_movie *addmv;
    add_user *addusr;
    add_rental *addrntl;
    add_payment *addpmnt;
    fastAdd *fadd;
    add_in_view *addInView;
    //--------

    upd_movie *update_movie;
    upd_user *update_user;
    upd_rental *update_rental;
    upd_payments *update_payments;
    upd_view *update_view;
    //--------
    queries *qrs;


    void CustomMenu(const QPoint &pos, QTableView *tableView, TableViewVariant tableVariant);

};
#endif // MAINWINDOW_H
