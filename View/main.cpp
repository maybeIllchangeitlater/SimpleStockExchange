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


///change username and password
/// documentation using Open API 2.0 aka Swagger
/// unit test coverage with boost tests
/// docker image for server


/// change create bid in view bid to display newly made transaction
///
/// large balance either not displayed corrrectly or not displayed correctly when other balance is 0
///
///
/// move response error from controllers to utility or controller common
