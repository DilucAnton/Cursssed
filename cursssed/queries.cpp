#include "queries.h"
#include "ui_queries.h"

queries::queries(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::queries)
{
    ui->setupUi(this);
}

queries::~queries()
{
    delete ui;
}

void queries::on_c1Button_clicked()
{
    QSqlQueryModel* c1model = qmod->query_c1();
    ui->tableView->setModel(c1model);
    ui->tableView->resizeColumnsToContents();
}


void queries::on_c2Button_clicked()
{
    QString value = ui->c2ComboBox->currentText();

    QSqlQueryModel* c2model = qmod->query_c2(value);

    ui->tableView->setModel(c2model);
    ui->tableView->resizeColumnsToContents();
}


void queries::on_c3Button_clicked()
{
    QSqlQueryModel* c3model = qmod->query_c3();

    ui->tableView->setModel(c3model);
    ui->tableView->resizeColumnsToContents();
}


void queries::on_anyButton_clicked()
{
    QString value = ui->anyComboBox->currentText();

    QSqlQueryModel* anyModel = qmod->query_Any(value);

    ui->tableView->setModel(anyModel);
    ui->tableView->resizeColumnsToContents();
}


void queries::on_allButton_clicked()
{
    QString value = ui->allComboBox->currentText();

    QSqlQueryModel* allModel = qmod->query_All(value);

    ui->tableView->setModel(allModel);
    ui->tableView->resizeColumnsToContents();
}


void queries::on_caseButton_clicked()
{
    QSqlQueryModel* caseModel = qmod->query_CASE();
    ui->tableView->setModel(caseModel);
    ui->tableView->resizeColumnsToContents();
}


void queries::on_manytablesButton_clicked()
{
    QSqlQueryModel* manTabModel = qmod->query_ManyTables();
    ui->tableView->setModel(manTabModel);
    ui->tableView->resizeColumnsToContents();
}


void queries::on_cor1Button_clicked()
{
    QSqlQueryModel* corModel = qmod->query_corr_1();
    ui->tableView->setModel(corModel);
    ui->tableView->resizeColumnsToContents();
}


void queries::on_cor2Button_clicked()
{
    QSqlQueryModel* corModel = qmod->query_corr_2();
    ui->tableView->setModel(corModel);
    ui->tableView->resizeColumnsToContents();
}


void queries::on_cor3Button_clicked()
{
    QSqlQueryModel* corModel = qmod->query_corr_3();
    ui->tableView->setModel(corModel);
    ui->tableView->resizeColumnsToContents();
}


void queries::on_viewButton_clicked()
{
    qmodel = new QSqlQueryModel();
    qmodel->setQuery("SELECT "
                         "\"Название фильма\", "
                         "\"Жанр\", "
                         "\"Год выпуска\", "
                         "\"Режиссер\", "
                         "\"Цена билета\", "
                         "\"Имя пользователя\", "
                         "\"Фамилия пользователя\", "
                         "\"Телефон\", "
                         "\"Дата возврата\", "
                         "\"Цена аренды\", "
                         "\"Статус оплаты\" "
                       "FROM "
                         "MovieRentalsView;");

    ui->tableView->setModel(qmodel);
    ui->tableView->resizeColumnsToContents();




}

