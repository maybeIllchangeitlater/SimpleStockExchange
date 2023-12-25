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

///change username and password
///fix line edit position on close (should be reset to first line)
///
/// move response error from controllers to utility or controller common
/// move all string raw literals to utility
///create Bid created, Bid fullfilled status messages as variables in utility
/// create separate static(?) class for server message parsing
/// correct architecture for server to send json to service through controller instead of just passing it down
/// view transaction and bids as table display
