#ifndef UPD_RENTAL_H
#define UPD_RENTAL_H

#include <QWidget>
#include "querymodel.h"

namespace Ui {
class upd_rental;
}

class upd_rental : public QWidget
{
    Q_OBJECT

public:
    explicit upd_rental(int rentalID, QWidget *parent = nullptr);
    ~upd_rental();
signals:
    void refresh();
private slots:
    void on_pushButton_clicked();

private:
    Ui::upd_rental *ui;

    int rentalID;
    void fillData();
    QueryModel *qmod;
};

#endif // UPD_RENTAL_H
