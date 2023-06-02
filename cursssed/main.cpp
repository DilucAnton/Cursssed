#include "mainwindow.h"
#include "connection_dialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *w = new MainWindow();
    connection_dialog *conn = new connection_dialog();

    conn->show();

    QObject::connect(
                conn, &connection_dialog::loginedSignal,
                w, &MainWindow::slotForConnect
                );
    QObject::connect(
                conn, &connection_dialog::loginedSignal,
                w, &MainWindow::setupButtons
                );

    return a.exec();
}
