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
    connect(ui->Logout, &QPushButton::clicked, this, [&](){
        client_.Disconnect();
        ui->ServerMessageInitScreen->setText("Signed Out");
        ui->LoggedAs->clear();
        SetNotLoginnedButtons();
    });
    connect(ui->CreateBid, &QPushButton::clicked, this, [&](){
       create_bid_pop_->exec();
    });
    connect(ui->Bids, &QPushButton::clicked, this, [&](){
       view_bid_pop_->exec();

    });
    connect(ui->Balance, &QPushButton::clicked, this, [&](){
       client_.CheckBalance();
       client_.WaitForResponse();
       ui->ServerMessageInitScreen->setText(QString::fromStdString(client_.CleanServerResponse()));
    });
    connect(reg_pop_.get(), &RegisterPopup::RegisterAttempt, this, &MainWindow::HandleRegisterAttempt);
    connect(log_pop_.get(), &LoginPopup::LoginAttempt, this, &MainWindow::HandleLoginAttempt);
    connect(create_bid_pop_.get(), &CreateBidPopup::MakeBid, this, &MainWindow::HandleCreateBid);
    connect(view_bid_pop_.get(), &ViewBids::ViewBid, this, &MainWindow::HandleViewBid);
    connect(view_bid_pop_.get(), &ViewBids::CancelBid, this, [&](const std::string bid_id){
        client_.CancelBid(bid_id);
        client_.WaitForResponse();
        auto error_msg = client_.CleanServerResponse();
        ui->ServerMessageInitScreen->setText(error_msg.empty()
                                             ? "Bid Cancelled"
                                             : QString::fromStdString(error_msg));
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
        ui->LoggedAs->setText(QString::fromStdString(username));
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
        ui->LoggedAs->setText(QString::fromStdString(username));
        SetLoginnedButtons();
    }else{
        ui->ServerMessageInitScreen->setText(QString::fromStdString(client_.CleanServerResponse()));
    }
    reg_pop_->hide();
    reg_pop_->close();
}

void MainWindow::HandleCreateBid(const std::string quantity, const std::string rate, const std::string bid_type)
{
    if(bid_type.find("Sell") != std::string::npos){
        client_.CreateSellBid(quantity, rate);
        client_.WaitForResponse();
        auto error_msg = client_.CleanServerResponse();
        ui->ServerMessageInitScreen->setText(error_msg.empty()
                                             ? "Bid Created"
                                             : QString::fromStdString(error_msg));
    }else if(bid_type.find("Buy") != std::string::npos){
        client_.CreateBuyBid(quantity, rate);
        client_.WaitForResponse();
        auto error_msg = client_.CleanServerResponse();
        ui->ServerMessageInitScreen->setText(error_msg.empty()
                                             ? "Bid Created"
                                             : QString::fromStdString(error_msg));
    }else{
        ui->ServerMessageInitScreen->setText("Invalid bid type");
    }
    create_bid_pop_->hide();
    create_bid_pop_->close();
}

void MainWindow::HandleViewBid(const std::string bid_type)
{
    if(bid_type.find("Sell") != std::string::npos){
        client_.GetMySellBids();
        client_.WaitForResponse();
        view_bid_pop_->ShowBids(client_.CleanServerResponse(), "Sell");
    }else if(bid_type.find("Buy") != std::string::npos){
        client_.GetMyBuyBids();
        client_.WaitForResponse();
        view_bid_pop_->ShowBids(client_.CleanServerResponse(), "Buy");
    }else{
        ui->ServerMessageInitScreen->setText("Invalid bid type");
    }
    create_bid_pop_->hide();
    create_bid_pop_->close();
}

void MainWindow::SetLoginnedButtons()
{
    ui->Login->setVisible(false);
    ui->Register->setVisible(false);

    ui->Logout->setVisible(true);
    ui->Balance->setVisible(true);
    ui->Bids->setVisible(true);
    ui->CreateBid->setVisible(true);
    ui->Transactions->setVisible(true);
    ui->DeleteMe->setVisible(true);
    ui->Quotations->setVisible(true);

    ui->layoutwidget->setCurrentIndex(1);
}

void MainWindow::SetNotLoginnedButtons()
{
    ui->Login->setVisible(true);
    ui->Register->setVisible(true);

    ui->Logout->setVisible(false);
    ui->Balance->setVisible(false);
    ui->Bids->setVisible(false);
    ui->CreateBid->setVisible(false);
    ui->Transactions->setVisible(false);
    ui->DeleteMe->setVisible(false);
    ui->Quotations->setVisible(false);

    ui->layoutwidget->setCurrentIndex(0);

}

void MainWindow::Connect()
{
    if(!client_.Connected()){
        client_.Connect("127.0.0.1", 5050);
        client_.WaitForResponse();
        ui->ServerMessageInitScreen->setText(QString::fromStdString(client_.CleanServerResponse()));
    }
}

