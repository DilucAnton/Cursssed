#ifndef UPD_VIEW_H
#define UPD_VIEW_H

#include <QWidget>

namespace Ui {
class upd_view;
}

class upd_view : public QWidget
{
    Q_OBJECT

public:
    explicit upd_view(int rentalID, QWidget *parent = nullptr);
    ~upd_view();

signals:
    void refresh();
private slots:
    void on_pushButton_clicked();

private:
    Ui::upd_view *ui;

    int rentalID;
    int movieID;
    int userID;
    void fillData();
};

#endif // UPD_VIEW_H
