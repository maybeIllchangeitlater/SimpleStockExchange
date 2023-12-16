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


///mutex MANAGING on disconnects and reconnects since the same q is used. Or recreate q lazy way
