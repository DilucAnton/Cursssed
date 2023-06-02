#include "connection_dialog.h"
#include "ui_connection_dialog.h"

connection_dialog::connection_dialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::connection_dialog)
{
    ui->setupUi(this);
}

connection_dialog::~connection_dialog()
{
    delete ui;
}



void connection_dialog::on_pushButton_clicked()
{
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setDatabaseName("cursssed");
    db.setHostName ("127.0.0.1") ;
    db.setUserName(ui->lineEdit->text());
    db.setPassword(ui->lineEdit_2->text());


    msg = new QMessageBox();
    bool isConnected = false;
    if (db.open())
    {

        username = ui->lineEdit->text();


        //msg->setText("Соединение установлено");
        isConnected = true;
        this->close();
        emit loginedSignal(username);

     }
    else {
        msg->setText("Ошибка подключения: " + db.lastError().text());
        isConnected = false;
        msg->show();
    }


    if(isConnected)
    {
        close();
    }
}


void connection_dialog::on_pushButton_2_clicked()
{
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setDatabaseName("cursssed");
    db.setHostName ("127.0.0.1") ;
    db.setUserName("postgres");
    db.setPassword("eve");


    msg = new QMessageBox();
    bool isConnected = false;
    if (db.open())
    {
        //msg->setText("Соединение установлено");
        isConnected = true;
        this->close();  
        emit loginedSignal("postgres");
     }
    else {
        msg->setText("Ошибка подключения: " + db.lastError().text());
        isConnected = false; 
        msg->show();

    }

    if(isConnected)
    {
        close();
    }


}

