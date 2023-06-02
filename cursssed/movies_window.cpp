#include "movies_window.h"
#include "ui_movies_window.h"

movies_window::movies_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::movies_window)
{
    ui->setupUi(this);

    //СВЯЗАНО С КОНТЕКСТ МЕНЮ:
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu); // разрешаем вызывать контекстное меню
    connect(ui->tableView,SIGNAL(customContextMenuRequested(QPoint)),SLOT(CustomMenuReq(QPoint)));
    //**
}

movies_window::~movies_window()
{
    delete ui;
}

void movies_window::on_pushButton_clicked()
{
    qmodel = new QSqlQueryModel();
    qmodel->setQuery("SELECT id ,title AS Название,genre AS Жанр, director AS Продюссер,release_year AS Год_выпуска,price AS Цена_билета FROM public.movies;");
    ui->tableView->setModel(qmodel);
    ui->tableView->hideColumn(0);
}


void movies_window::on_pushButton_2_clicked()
{
    addmv = new add_movie();
    addmv->show();
}



void movies_window::on_pushButton_5_clicked()
{
    this->close();
}



//ВСЕ ЧТО СВЯЗАНО С КОНТЕКСТ МЕНЮ:
void movies_window::CustomMenuReq(QPoint pos)
{
    QModelIndex index = ui->tableView->indexAt(pos);

    globint = ui->tableView->model()->data(ui->tableView->model()->index(index.row(),0)).toInt();
    //обращаемся к таблице и вытаскиваем первые значения столбца
    QMenu* menu = new QMenu(this);
    QAction* change = new QAction("Изменить", this);




    QAction* del = new QAction("Удалить", this);

    connect(del, SIGNAL(triggered()), this, SLOT(deleteFragment()) );

    menu->addAction(change);
    menu->addAction(del);
    menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
}

void movies_window::deleteFragment()
{
    QSqlQuery* query = new QSqlQuery();
    //query->prepare("select public.delete_movie(?);");
    query->prepare("DELETE FROM movies WHERE id=?");
    query->bindValue(0,globint);

    msg = new QMessageBox();
    if (query->exec()) {
        // запрос выполнен успешно
        on_pushButton_clicked();
        msg->setText("Запись удалена");
    } else {
        // произошла ошибка выполнения запроса
        msg->setText("Ошибка: " + query->lastError().text());
    }
    msg->show();
}
