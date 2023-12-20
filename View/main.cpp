#include "mainwindow.hpp"
#include "../Client/Client.hpp"


#include <QApplication>

int main(int argc, char *argv[])
{
    s21::Client client;
    QApplication a(argc, argv);
    MainWindow w(client);
    w.show();
    return a.exec();
}


///fix display of transactions
/// update rate and quantity of transactions
/// update of balance during transaction
/// documentation using Open API 2.0 aka Swagger
/// unit test coverage with boost tests
/// docker image for server
