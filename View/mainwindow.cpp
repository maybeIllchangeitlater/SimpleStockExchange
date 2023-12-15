#include "mainwindow.hpp"
#include "./ui_mainwindow.h"
#include <boost/thread.hpp>
#include <boost/chrono.hpp>


MainWindow::MainWindow(s21::Client &client, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
    , client_(client)
{
    ui->setupUi(this);
    SetNotLoginnedButtons();
    ui->ServerMessageInitScreen->setWordWrap(true);
    connect(ui->Register, &QPushButton::clicked, this, [&](){
        Connect();
        reg_pop_->exec();
    });
    connect(ui->Login, &QPushButton::clicked, this, [&](){
        Connect();
        log_pop_->exec();
    });
    connect(reg_pop_.get(), &RegisterPopup::RegisterAttempt, this, &MainWindow::HandleRegisterAttempt);
    connect(log_pop_.get(), &LoginPopup::LoginAttempt, this, &MainWindow::HandleLoginAttempt);
    connect(ui->Logout, &QPushButton::clicked, this, [&](){
        client_.Disconnect();
        std::cout << "turning on log buttons\n";
        SetNotLoginnedButtons();
    });
    Connect();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::HandleLoginAttempt(const std::string username, const std::string password)
{
    client_.LogIn(username, password);
    std::cout << "attempting login, waiting for response\n";
    client_.WaitForResponse();
    std::cout << "response came\n";
    if(client_.CheckStatus()){
        client_.Authorize();
        ui->ServerMessageInitScreen->setText(QString::fromStdString("Loged in as " + username));
        SetLoginnedButtons();
    }else{
        ui->ServerMessageInitScreen->setText(QString::fromStdString(client_.CleanServerResponse()));
    }
    log_pop_->hide();
    log_pop_->close();
}

void MainWindow::HandleRegisterAttempt(const std::string username, const std::string password, const std::string balance)
{
    client_.Register(username, password, balance);
    client_.WaitForResponse();
    if(client_.CheckStatus()){
        client_.Authorize();
        ui->ServerMessageInitScreen->setText(QString::fromStdString("Registered with username " + username));
        SetLoginnedButtons();
    }else{
        ui->ServerMessageInitScreen->setText(QString::fromStdString(client_.CleanServerResponse()));
    }
    reg_pop_->hide();
    reg_pop_->close();
}

void MainWindow::SetLoginnedButtons()
{
    ui->Login->setVisible(false);
    ui->Register->setVisible(false);
    ui->Logout->setVisible(true);
}

void MainWindow::SetNotLoginnedButtons()
{
    ui->Login->setVisible(true);
    ui->Register->setVisible(true);
    ui->Logout->setVisible(false);
    std::cout << "butooooons\n";
}

void MainWindow::Connect()
{
    std::cout << "Not dc'd yet\n";
    if(!client_.Connected()){
        std::cout << "trying to connect\n";
        client_.Connect("127.0.0.1", 5050);
        std::cout << "connected\n";
        if(client_.Connected()){
            std::cout << "connected-connected\n";
        }
        client_.WaitForResponse();
        std::cout << "tsq mutex didnt fail\n";
        ui->ServerMessageInitScreen->setText(QString::fromStdString(client_.CleanServerResponse()));
    }
}

