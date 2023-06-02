#ifndef UPD_MOVIE_H
#define UPD_MOVIE_H

#include <QWidget>

namespace Ui {
class upd_movie;
}

class upd_movie : public QWidget
{
    Q_OBJECT

public:
    explicit upd_movie(int movieID, QWidget *parent = nullptr);
    ~upd_movie();

private slots:
    void on_pushButton_clicked();
signals:
    void refresh();
private:
    void fillData();

    Ui::upd_movie *ui;

    int movieID;
};

#endif // UPD_MOVIE_H
