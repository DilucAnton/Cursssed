#ifndef QUERYMODEL_H
#define QUERYMODEL_H
#include <QSqlQueryModel>

class QueryModel
{
public:
    QueryModel();
    QSqlQueryModel* userSelectAllList();
    QSqlQueryModel* moviesSelectAllList(); 
    QSqlQueryModel* rentalsSelectAllList();
    QSqlQueryModel* rentalsSelect();
    QSqlQueryModel* query_c1();
    QSqlQueryModel* query_c2(QString value);
    QSqlQueryModel* query_c3();
    QSqlQueryModel* query_Any(QString value);
    QSqlQueryModel* query_All(QString value);
    QSqlQueryModel* query_CASE();
    QSqlQueryModel* query_ManyTables();
    QSqlQueryModel* query_corr_1();
    QSqlQueryModel* query_corr_2();
    QSqlQueryModel* query_corr_3();


};



#endif // QUERYMODEL_H
