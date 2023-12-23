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


///transactions tab and change username and password
/// documentation using Open API 2.0 aka Swagger
/// unit test coverage with boost tests
/// docker image for server


/// update of balance during transaction
///
/// add create bid option to view bids
///
/// large balance either not displayed corrrectly or not displayed correctly when other balance is 0
