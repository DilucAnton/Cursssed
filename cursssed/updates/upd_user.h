#ifndef UPD_USER_H
#define UPD_USER_H

#include <QWidget>

namespace Ui {
class upd_user;
}

class upd_user : public QWidget
{
    Q_OBJECT

public:
    explicit upd_user(int userID, QWidget *parent = nullptr);
    ~upd_user();

private slots:
    void on_pushButton_clicked();
signals:
    void refresh();
private:
    Ui::upd_user *ui;

    void fillData();
    int userID;
};

#endif // UPD_USER_H
