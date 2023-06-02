#ifndef UPD_PAYMENTS_H
#define UPD_PAYMENTS_H

#include <QWidget>
#include "querymodel.h"

namespace Ui {
class upd_payments;
}

class upd_payments : public QWidget
{
    Q_OBJECT

public:
    explicit upd_payments(int paymentID,QWidget *parent = nullptr);
    ~upd_payments();
signals:
    void refresh();
private slots:

    void on_pushButton_clicked();

private:
    Ui::upd_payments *ui;

    int paymentID;
    void fillData();
    QueryModel *qmod;
};

#endif // UPD_PAYMENTS_H
