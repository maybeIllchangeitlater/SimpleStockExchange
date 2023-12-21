#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include "../Client/Client.hpp"
#include "LoginPopup.hpp"
#include "RegisterPopup.hpp"
#include "CreateBidPopup.hpp"
#include "BidTransactionMadePopup.hpp"
#include "ViewBids.hpp"
#include "DeleteAccountPopup.hpp"
#include "UpdateBidPopup.hpp"
#include <memory.h>
#include <boost/make_unique.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(s21::Client &client, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void HandleLoginAttempt(const std::string username, const std::string password);
    void HandleRegisterAttempt(const std::string username, const std::string password, const std::string balance);
    void HandleCreateBid(const std::string quantity, const std::string rate, const std::string bid_type);
    void HandleViewBid(const std::string bid_type);

private:
    void SetLoginnedButtons();
    void SetNotLoginnedButtons();
    void Connect();
    Ui::MainWindow *ui;
    std::unique_ptr<LoginPopup> log_pop_ = boost::make_unique<LoginPopup>();
    std::unique_ptr<ViewBids> view_bid_pop_ = boost::make_unique<ViewBids>();
    std::unique_ptr<RegisterPopup> reg_pop_ = boost::make_unique<RegisterPopup>();
    std::unique_ptr<UpdateBidPopup> upd_bid_pop_ = boost::make_unique<UpdateBidPopup>();
    std::unique_ptr<CreateBidPopup> create_bid_pop_ = boost::make_unique<CreateBidPopup>();
    std::unique_ptr<DeleteAccountPopup> delete_account_pop_ = boost::make_unique<DeleteAccountPopup>();
    std::unique_ptr<BidTransactionMadePopup> newtrans_pop_ = boost::make_unique<BidTransactionMadePopup>();
    s21::Client &client_;
};
#endif // MAINWINDOW_HPP
