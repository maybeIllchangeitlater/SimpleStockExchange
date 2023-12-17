#include "mainwindow.hpp"
#include "../Client/Client.hpp"


#include <QApplication>

int main(int argc, char *argv[])
{
    s21::Client client;
//    client.Connect("127.0.0.1", 5050);
    QApplication a(argc, argv);
    MainWindow w(client);
    w.show();
    return a.exec();
}


///get user by id breaks everything, (get user raw?) Bids are never cancelled sometimes not send (long fix)
