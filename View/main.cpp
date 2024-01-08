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


/// documentation using Open API 2.0 aka Swagger
/// unit test coverage with boost tests
/// docker image for server

/// move response error from controllers to utility or controller common
/// move all string raw literals to utility
///create Bid created, Bid fullfilled status messages as variables in utility
///token authentifiaction
/// signout on exit
/// disconnect for server response timer
