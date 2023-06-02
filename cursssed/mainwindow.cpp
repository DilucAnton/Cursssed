#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

UserRole MainWindow::userRoles()
{

    if (username == "adm" || username=="postgres") {
        return Administrator;
    } else if (username == "btr") {
        return BasicUser;
    } else {
        return Viewer;
    }
}

void MainWindow::setupButtons() {

    UserRole userRole = userRoles();

    // Найти кнопки по их идентификаторам и настроить доступность
    QPushButton* fastAddButton = findChild<QPushButton*>("fastAddButton");
    QAction* addAction1  = findChild<QAction*>("actionAdd_Movie");
    QAction* addAction2  = findChild<QAction*>("actionAdd_User");
    QAction* addAction3  = findChild<QAction*>("actionAdd_Rental");
    QAction* addAction4  = findChild<QAction*>("actionAdd_Payment");
    QAction* addActionSQL  = findChild<QAction*>("action");
    QAction* actionAdd_InView = findChild<QAction*>("actionAdd_InView");



    if (fastAddButton && addAction1 && addAction2 && addAction3 && addAction4 && addActionSQL && actionAdd_InView) {
        switch (userRole) {
            case Administrator:
                fastAddButton->setEnabled(true);
                addAction1->setEnabled(true);
                addAction2->setEnabled(true);
                addAction3->setEnabled(true);
                addAction4->setEnabled(true);
                addActionSQL->setEnabled(true);
                actionAdd_InView->setEnabled(true);
                break;
            case BasicUser:
                fastAddButton->setEnabled(true);
                addAction1->setEnabled(true);
                addAction2->setEnabled(true);
                addAction3->setEnabled(true);
                addAction4->setEnabled(true);
                addActionSQL->setEnabled(true);
                actionAdd_InView->setEnabled(true);

                break;
            case Viewer:
                fastAddButton->setEnabled(false);
                addAction1->setEnabled(false);
                addAction2->setEnabled(false);
                addAction3->setEnabled(false);
                addAction4->setEnabled(false);
                addActionSQL->setEnabled(true);
                actionAdd_InView->setEnabled(false);
                break;
            default:
                fastAddButton->setEnabled(false);
                addAction1->setEnabled(false);
                addAction2->setEnabled(false);
                addAction3->setEnabled(false);
                addAction4->setEnabled(false);
                addActionSQL->setEnabled(true);
                break;
        }
    }
}


//слот для подключения к бд
void MainWindow::slotForConnect(QString username)
{
    this->show();
    this->username = username;
    scalAndVecFunc();


    on_pushButton_4_clicked();
    on_pushButton_3_clicked();
    on_pushButton_2_clicked();
    on_pushButton_clicked();
    on_pushButton_5_clicked();

}


void MainWindow::on_actionConnect_to_DB_triggered()
{
    con_dlg = new connection_dialog();
    con_dlg->show();
    //username = con_dlg->userRole;

}

//запросы
void MainWindow::on_action_triggered()
{
    qrs = new queries();
    qrs->show();
}

// кнопка для добавления фильма
void MainWindow::on_actionAdd_Movie_triggered()
{
    addmv = new add_movie();
    connect(addmv,SIGNAL(refresh()),this,SLOT(movie_refresh()));
    connect(addmv,SIGNAL(refresh()),this,SLOT(scalAndVecFunc()));
    addmv->show();
}

void MainWindow::on_actionAdd_User_triggered()
{
    addusr = new add_user();
    connect(addusr,SIGNAL(refresh()),this,SLOT(users_refresh()));
    connect(addusr,SIGNAL(refresh()),this,SLOT(scalAndVecFunc()));

    addusr->show();
}

void MainWindow::on_actionAdd_Rental_triggered()
{
    addrntl = new add_rental();
    connect(addrntl,SIGNAL(refresh()),this,SLOT(rentals_refresh()));
    connect(addrntl,SIGNAL(refresh()),this,SLOT(scalAndVecFunc()));
    addrntl->show();
}

void MainWindow::on_actionAdd_Payment_triggered()
{
    addpmnt = new add_payment();
    connect(addpmnt,SIGNAL(refresh()),this,SLOT(payments_refresh()));
    connect(addpmnt,SIGNAL(refresh()),this,SLOT(scalAndVecFunc()));
    addpmnt->show();
}

void MainWindow::on_actionAdd_InView_triggered()
{
    addInView = new add_in_view();
    connect(addInView,SIGNAL(refresh()),this,SLOT(view_refresh()));
    connect(addInView,SIGNAL(refresh()),this,SLOT(scalAndVecFunc()));
    addInView->show();
}

// обновление фильма по нажатию кнопки, было нужно для закрытия окна

void MainWindow::movie_refresh()
{
    on_pushButton_clicked();
}
void MainWindow::users_refresh()
{
    on_pushButton_2_clicked();
}
void MainWindow::rentals_refresh()
{
    on_pushButton_3_clicked();
}

void MainWindow::payments_refresh()
{
    on_pushButton_4_clicked();
}
void MainWindow::view_refresh()
{
    on_pushButton_5_clicked();
}




// кнопка movies для показа фильмов
void MainWindow::on_pushButton_clicked()
{
    qmodel = new QSqlQueryModel();
    qmodel->setQuery("SELECT id ,title AS Название,genre AS Жанр, director AS Режиссер,release_year AS \"Год выпуска\",price AS \"Цена билета\" FROM public.movies;");
    ui->table_movies->setModel(qmodel);
    ui->table_movies->hideColumn(0);
    ui->table_movies->resizeColumnsToContents();

    ui->tabWidget->setCurrentIndex(1);
}

// кнопка для показа юзеров
void MainWindow::on_pushButton_2_clicked()
{
    qmodel = new QSqlQueryModel();
    qmodel->setQuery("SELECT id ,first_name AS Имя,last_name AS Фамилия, phone AS \"Номер телефона\" FROM public.users;");
    ui->table_users->setModel(qmodel);
    ui->table_users->hideColumn(0);
    ui->table_users->resizeColumnsToContents();


    ui->tabWidget->setCurrentIndex(2);
}


void MainWindow::on_pushButton_3_clicked()
{
    qmodel = new QSqlQueryModel();
    qmodel->setQuery("SELECT r.id, CONCAT(u.first_name, ' ', u.last_name) AS \"Имя пользователя\", m.title AS \"Название фильма\", r.checkout_date AS \"Дата получения\", r.return_date AS \"Дата возврата\", r.total_cost AS \"Цена Аренды\", r.status AS \"Статус\", CASE WHEN r.payment_status = TRUE THEN 'оплачено' ELSE 'не оплачено' END AS \"Статус оплаты\" FROM Rentals r JOIN Users u ON r.user_id = u.id JOIN Movies m ON r.movie_id = m.id;");
    ui->table_rentals->setModel(qmodel);
    ui->table_rentals->hideColumn(0);
    ui->table_rentals->resizeColumnsToContents();

    ui->tabWidget->setCurrentIndex(3);
}


void MainWindow::on_pushButton_4_clicked() {
    qmodel = new QSqlQueryModel();
    qmodel->setQuery("SELECT p.id ,CONCAT(u.first_name, ' ', u.last_name, ' - ', m.title) AS \"Имя пользователя и фильм\", p.amount AS \"Сумма\", p.date AS \"Дата платежа\" FROM Payments p JOIN Rentals r ON p.rental_id = r.id JOIN Users u ON r.user_id = u.id JOIN Movies m ON r.movie_id = m.id;");
    ui->table_payments->setModel(qmodel);
    ui->table_payments->hideColumn(0);
    ui->table_payments->resizeColumnsToContents();


    ui->tabWidget->setCurrentIndex(4);
}


void MainWindow::on_fastAddButton_clicked()
{


    fadd = new fastAdd();

    connect(fadd,SIGNAL(refresh()),this,SLOT(scalAndVecFunc()));

    connect(fadd,SIGNAL(refresh()),this,SLOT(payments_refresh()));

    fadd->show();

}


void MainWindow::on_pushButton_5_clicked()
{

    qmodel = new QSqlQueryModel();
    qmodel->setQuery("SELECT * FROM MovieRentalsView;");
    ui->table_view->setModel(qmodel);
    ui->table_view->resizeColumnsToContents();


    ui->table_view->hideColumn(0);
    ui->table_view->hideColumn(1);
    ui->table_view->hideColumn(2);

    ui->tabWidget->setCurrentIndex(0);

}






QString TableName(TableViewVariant TableVariant)
{
    switch(TableVariant){
    case movies:
        return "movies";
        break;
    case users:
        return "users";
        break;
    case rentals:
        return "rentals";
        break;
    case payments:
        return "payments";
        break;
    case view:
        return "view";
        break;
    }
    return 0;
}

void MainWindow::CustomMenu(const QPoint &pos, QTableView *tableView, TableViewVariant tableVariant)
{
    UserRole role = userRoles();

    if (role == Viewer)
    {
        return;
    }

    if(tableVariant == view)
    {
        QModelIndex index = tableView->currentIndex();
        int RentID= tableView->model()->data(tableView->model()->index(index.row(),2)).toInt();

        QMenu *menu = new QMenu(this);

        QAction *ModifyRec = new QAction("Изменить...", this);
        QAction *DeleteRec = new QAction("Удалить", this);

        connect(ModifyRec, &QAction::triggered,this, [=]() {
            modActView(RentID);
        });
        connect(DeleteRec, &QAction::triggered,this, [=]() {
            delActView(RentID);
        });

        menu->addAction(ModifyRec);
        menu->addAction(DeleteRec);
        menu->popup(tableView->viewport()->mapToGlobal(pos));

        return;
    }



    QModelIndex index = tableView->currentIndex();
    int temp_ID = tableView->model()->data(tableView->model()->index(index.row(),0)).toInt();

    //QString CurrentTable = TableName(tableVariant);

    QMenu *menu = new QMenu(this);

    QAction *ModifyRec = new QAction("Изменить...", this);
    QAction *DeleteRec = new QAction("Удалить", this);

    connect(ModifyRec, &QAction::triggered,this, [=]() {
        modAction(temp_ID, tableVariant);
    });
    connect(DeleteRec, &QAction::triggered,this, [=]() {
        delAction(temp_ID, tableVariant);
    });

    menu->addAction(ModifyRec);
    menu->addAction(DeleteRec);
    menu->popup(tableView->viewport()->mapToGlobal(pos));

}

void MainWindow::modActView(int RentID)
{
    update_view = new upd_view(RentID);

    connect(update_view,SIGNAL(refresh()),this,SLOT(view_refresh()));
    connect(update_view,SIGNAL(refresh()),this,SLOT(scalAndVecFunc()));
    update_view->show();

}

void MainWindow::delActView(int RentID)
{

    QSqlQuery query;
    query.prepare("DELETE FROM MovieRentalsView WHERE \"ID аренды\" = :RentID ;");
    query.bindValue(":RentID", RentID);

    if (query.exec()){
            on_pushButton_5_clicked();
            scalAndVecFunc();

        }
    else {
        QMessageBox msg;
        msg.setText("Не получилось удалить элемент таблицы");
        qDebug() << query.lastError().text();
    }

}
void MainWindow::modAction(int selectedID, TableViewVariant selectedTable)
{
    switch(selectedTable) {
            case movies: {
                update_movie = new upd_movie(selectedID);
                connect(update_movie,SIGNAL(refresh()),this,SLOT(movie_refresh()));
                connect(update_movie,SIGNAL(refresh()),this,SLOT(scalAndVecFunc()));
                update_movie->show();
                break;
            }
            case users: {
                update_user = new upd_user(selectedID);
                connect(update_user,SIGNAL(refresh()),this,SLOT(users_refresh()));
                connect(update_user,SIGNAL(refresh()),this,SLOT(scalAndVecFunc()));
                update_user->show();
                break;
            }
            case rentals: {
                update_rental = new upd_rental(selectedID);
                connect(update_rental,SIGNAL(refresh()),this,SLOT(rentals_refresh()));
                connect(update_rental,SIGNAL(refresh()),this,SLOT(scalAndVecFunc()));
                update_rental->show();
                break;
            }
            case payments: {
                update_payments = new upd_payments(selectedID);
                connect(update_payments,SIGNAL(refresh()),this,SLOT(payments_refresh()));
                connect(update_payments,SIGNAL(refresh()),this,SLOT(scalAndVecFunc()));
                update_payments->show();
                break;
            }
             case view:{

                break;
              }



        }

}


void MainWindow::delAction(int selectedID, TableViewVariant selectedTable)
{
    QString Table = TableName(selectedTable);

    QSqlQuery query;


//    QString str = QString("SELECT delete_" +Table+ "(%1)").arg(selectedID);
//       qDebug() << str;
    query.prepare("SELECT public.delete_"+Table+"(:ID)");
    query.bindValue(":ID", selectedID);


    if (query.exec()){
        switch(selectedTable) {
            case movies:
                on_pushButton_clicked();
                scalAndVecFunc();
                break;
            case users:
                on_pushButton_2_clicked();
                scalAndVecFunc();
                break;
            case rentals:
                on_pushButton_3_clicked();
                scalAndVecFunc();
                break;
            case payments:
                on_pushButton_4_clicked();
                scalAndVecFunc();
                break;
             case view:
                break;

            }
        }
    else {
        QMessageBox msg;
        msg.setText("Не получилось удалить элемент таблицы");
        qDebug() << query.lastError().text();
    }
}








void MainWindow::on_table_movies_customContextMenuRequested(const QPoint &pos)
{
   CustomMenu(pos,ui->table_movies,TableViewVariant::movies);
}


void MainWindow::on_table_users_customContextMenuRequested(const QPoint &pos)
{
   CustomMenu(pos,ui->table_users,TableViewVariant::users);
}

void MainWindow::on_table_rentals_customContextMenuRequested(const QPoint &pos)
{
   CustomMenu(pos,ui->table_rentals,TableViewVariant::rentals);
}


void MainWindow::on_table_payments_customContextMenuRequested(const QPoint &pos)
{
   CustomMenu(pos,ui->table_payments,TableViewVariant::payments);
}


void MainWindow::on_table_view_customContextMenuRequested(const QPoint &pos)
{
    CustomMenu(pos,ui->table_view,TableViewVariant::view);
}

void MainWindow::scalAndVecFunc()
{

    QSqlQuery query;
    query.prepare("select get_most_rented_movie();");
    query.exec();
    if (query.next())
    {
        ui->movieLabel->setText(query.value(0).toString());
    }

    query.prepare("select get_current_rental_count();");
    query.exec();
    if (query.next())
    {
        ui->rentalsLabel->setText(query.value(0).toString());
    }

    query.prepare("select get_highest_rental_cost();");
    query.exec();
    if (query.next())
    {
        ui->costLabel->setText(query.value(0).toString()+".00");
    }
}














