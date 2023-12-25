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
    ConnectToPopups();
    ConnectToHandlers();
    Connect();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::HandleLoginAttempt(const std::string username, const std::string password)
{
    client_.LogIn(username, password);
    client_.WaitForResponse();
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

void MainWindow::HandleRegisterAttempt(const std::string username, const std::string password,
                                       const std::string balance_usd, const std::string balance_rub)
{
    client_.Register(username, password, balance_usd, balance_rub);
    if(!WaitForServer()){
        return;
    }
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
    create_bid_pop_->hide();
    create_bid_pop_->close();
    bid_type.find("Sell") != std::string::npos
            ? client_.CreateSellBid(quantity, rate)
            : client_.CreateBuyBid(quantity, rate);

    if(!WaitForServer()){
        return;
    }
    if(!client_.CheckStatus()){
        ui->ServerMessageInitScreen->setText(QString::fromStdString(client_.CleanServerResponse()));
        return;
    }

    auto server_response = client_.Inbox().PopFront().second;
    if(!client_.CheckIfTransactionsWereMade(server_response)){
        ui->ServerMessageInitScreen->setText(
            QString::fromStdString(client_.CleanBidCreatedResponse(server_response)));
        if(view_bid_pop_->isVisible()){
                view_bid_pop_->InsertNewBid(server_response);
            }
    }else{
        server_response = newtrans_pop_->DisplayNewTransactions(server_response);
    ui->ServerMessageInitScreen->setText(QString::fromStdString(server_response.empty()
                                                                ? "Bid fullfilled"
                                                                : server_response));
        if(view_bid_pop_->isVisible()){
            view_bid_pop_->InsertNewBid(server_response);
        }
    newtrans_pop_->exec();
    }

}

void MainWindow::HandleViewBid(const std::string bid_type)
{
    if(bid_type.find("Sell") != std::string::npos){
        client_.GetMySellBids();
        if(!WaitForServer()){
            return;
        }
        view_bid_pop_->ShowBids(client_.CleanServerResponse(), "Sell");
    }else if(bid_type.find("Buy") != std::string::npos){
        client_.GetMyBuyBids();
        if(!WaitForServer()){
            return;
        }
        view_bid_pop_->ShowBids(client_.CleanServerResponse(), "Buy");
    }else{
        ui->ServerMessageInitScreen->setText("Invalid bid type");
    }
    create_bid_pop_->hide();
    create_bid_pop_->close();
}

void MainWindow::HandleUpdateBid(const std::string bid_id, const std::string bid_rate,
                                 const std::string bid_quantity, int index)
{
    client_.UpdateBidQuantity(bid_id, bid_quantity);
    upd_bid_pop_->hide();
    upd_bid_pop_->close();
    if(!WaitForServer()){
        return;
    }
    if(!client_.CheckStatus()){
        ui->ServerMessageInitScreen->setText(QString::fromStdString(client_.CleanServerResponse()));
        return;
    }
    client_.Inbox().PopFront();
    client_.UpdateBidRate(bid_id, bid_rate);
    if(!WaitForServer()){
        return;
    }
    if(!client_.CheckStatus()){
        ui->ServerMessageInitScreen->setText(QString::fromStdString(client_.CleanServerResponse()));
        return;
    }

    std::string server_response = client_.Inbox().PopFront().second;
    std::string detailed_bid = client_.ExtractDetailedBidInfoRaw(server_response);
    std::string display_msg = detailed_bid.empty()
            ? ""
            : client_.ExtractCleanBidUpdateResponse(detailed_bid);
    if(!client_.CheckIfTransactionsWereMade(server_response)){
        ui->ServerMessageInitScreen->setText(
            QString::fromStdString(display_msg));
        if(view_bid_pop_->isVisible()){
                view_bid_pop_->InsertUpdatedBidBack(detailed_bid, index);
            }
    }else{
        std::cout << server_response << "\n" << detailed_bid << "\n" << display_msg << "\n";
        if(server_response.find(']') == std::string::npos){
            server_response += "}]";
        }
        newtrans_pop_->DisplayNewTransactions(server_response);
        ui->ServerMessageInitScreen->setText(QString::fromStdString(display_msg.empty()
                                                                ? "Bid fullfilled"
                                                                : display_msg));
        if(view_bid_pop_->isVisible()){
            view_bid_pop_->InsertUpdatedBidBack(detailed_bid, index);
        }
        newtrans_pop_->exec();
    }
}

bool MainWindow::WaitForServer()
{
    if(!client_.WaitForResponse()){
        SetNotLoginnedButtons();
        ui->LoggedAs->clear();
        ui->ServerMessageInitScreen->setText("Lost connection to Server");
        return false;
    }
    return true;
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
        if(!WaitForServer()){
            return;
        }
        ui->ServerMessageInitScreen->setText(QString::fromStdString(client_.CleanServerResponse()));
    }
}

void MainWindow::ConnectToHandlers()
{
    connect(reg_pop_.get(), &RegisterPopup::RegisterAttempt, this, &MainWindow::HandleRegisterAttempt);
    connect(log_pop_.get(), &LoginPopup::LoginAttempt, this, &MainWindow::HandleLoginAttempt);
    connect(create_bid_pop_.get(), &CreateBidPopup::MakeBid, this, &MainWindow::HandleCreateBid);
    connect(upd_bid_pop_.get(), &UpdateBidPopup::UpdateBid, this, &MainWindow::HandleUpdateBid);
    connect(view_bid_pop_.get(), &ViewBids::ViewBid, this, &MainWindow::HandleViewBid);

    connect(ui->Logout, &QPushButton::clicked, this, [&](){
        client_.Disconnect();
        ui->ServerMessageInitScreen->setText("Signed Out");
        ui->LoggedAs->clear();
        SetNotLoginnedButtons();
    });
    connect(ui->Balance, &QPushButton::clicked, this, [&](){
       client_.CheckBalance();
       if(!WaitForServer()){
           return;
       }
       ui->ServerMessageInitScreen->setText(QString::fromStdString(client_.CleanServerResponse()).
                                            replace("\"", "").replace(",", "\n"));
    });

    connect(delete_account_pop_.get(), &DeleteAccountPopup::DeleteAccount, this, [&](){
       client_.DeleteMe();
       ui->LoggedAs->clear();
       ui->ServerMessageInitScreen->setText("Account Deleted");
       SetNotLoginnedButtons();
    });
    connect(view_bid_pop_.get(), &ViewBids::CancelBid, this, [&](const std::string bid_id){
        client_.CancelBid(bid_id);
        if(!WaitForServer()){
            return;
        }
        auto error_msg = client_.CleanServerResponse();
        ui->ServerMessageInitScreen->setText(error_msg.empty()
                                             ? "Bid Cancelled"
                                             : QString::fromStdString(error_msg));
    });
    connect(view_bid_pop_.get(), &ViewBids::UpdateBid, this, [&](const std::string bid_id,
            const std::string bid_rate, const std::string bid_quantity, int bid_index){
         upd_bid_pop_->SetParameters(bid_id, bid_rate, bid_quantity, bid_index);
         upd_bid_pop_->exec();
    });
    connect(viewtrans_pop_.get(), &ViewTransactionsPopup::ShowBuyTransactions, this, [&](){
        client_.GetMyBuyTransactions();
        if(!WaitForServer()){
            return;
        }
        if(!client_.CheckStatus()){
             ui->ServerMessageInitScreen->setText(QString::fromStdString(client_.CleanServerResponse()));
        }else{
            viewtrans_pop_->DisplayTransactions("Buy", client_.CleanServerResponse());
        }
    });
    connect(viewtrans_pop_.get(), &ViewTransactionsPopup::ShowSellTransactions, this, [&](){
        client_.GetMySellTransactions();
        if(!WaitForServer()){
            return;
        }
        if(!client_.CheckStatus()){
             ui->ServerMessageInitScreen->setText(QString::fromStdString(client_.CleanServerResponse()));
        }else{
            viewtrans_pop_->DisplayTransactions("Sell", client_.CleanServerResponse());
        }
    });
}

void MainWindow::ConnectToPopups()
{
    connect(ui->Register, &QPushButton::clicked, this, [&](){
        Connect();
        reg_pop_->exec();
    });
    connect(ui->Login, &QPushButton::clicked, this, [&](){
        Connect();
        log_pop_->exec();
    });
    connect(ui->CreateBid, &QPushButton::clicked, this, [&](){
       create_bid_pop_->exec();
    });
    connect(view_bid_pop_.get(), &ViewBids::CreateBid, this, [&](){
        create_bid_pop_->exec();
    });
    connect(ui->Bids, &QPushButton::clicked, this, [&](){
       view_bid_pop_->exec();

    });
    connect(ui->DeleteMe, &QPushButton::clicked, this, [&](){
        delete_account_pop_->exec();
    });
    connect(ui->Transactions, &QPushButton::clicked, this, [&](){
        viewtrans_pop_->exec();
    });
}

