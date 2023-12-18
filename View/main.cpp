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


///fix view balance, add transaction buisness logic
