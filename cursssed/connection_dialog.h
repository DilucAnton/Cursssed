#ifndef CONNECTION_DIALOG_H
#define CONNECTION_DIALOG_H

#include <QWidget>
#include <QtSql>
#include <QMessageBox>


namespace Ui {
class connection_dialog;
}

class connection_dialog : public QWidget
{
    Q_OBJECT

public:
    explicit connection_dialog(QWidget *parent = nullptr);
    ~connection_dialog();
    QMessageBox *msg;


    QString username;

signals:
    void loginedSignal(QString username);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::connection_dialog *ui;
    QSqlDatabase db;

};

#endif // CONNECTION_DIALOG_H
