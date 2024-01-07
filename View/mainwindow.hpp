#ifndef SIMPLESTOCKEXCHANGE_VIEW_MAINWINDOW_HPP
#define SIMPLESTOCKEXCHANGE_VIEW_MAINWINDOW_HPP

#include <QMainWindow>
#include "../Client/Client.hpp"
#include "LoginPopup.hpp"
#include "RegisterPopup.hpp"
#include "CreateBidPopup.hpp"
#include "BidTransactionMadePopup.hpp"
#include "ViewBids.hpp"
#include "ViewTransactionsPopup.hpp"
#include "UpdateBidPopup.hpp"
#include "UserSettings.hpp"
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
    void HandleRegisterAttempt(const std::string username, const std::string password, const std::string balance_usd,
                               const std::string balance_rub);
    void HandleCreateBid(const std::string quantity, const std::string rate, const std::string bid_type);
    void HandleViewBid(const std::string bid_type);
    void HandleUpdateBid(const std::string bid_id, const std::string bid_rate,
                         const std::string bid_quantity, int index);

private:
    bool WaitForServer();
    void SetLoginnedButtons();
    void SetNotLoginnedButtons();
    void Connect();
    void ConnectToHandlers();
    void ConnectToSettingsHandlers();
    void ConnectToPopups();
    Ui::MainWindow *ui;
    std::unique_ptr<LoginPopup> log_pop_ = boost::make_unique<LoginPopup>();
    std::unique_ptr<ViewBids> view_bid_pop_ = boost::make_unique<ViewBids>();
    std::unique_ptr<RegisterPopup> reg_pop_ = boost::make_unique<RegisterPopup>();
    std::unique_ptr<UserSettings> user_settings_ = boost::make_unique<UserSettings>();
    std::unique_ptr<UpdateBidPopup> upd_bid_pop_ = boost::make_unique<UpdateBidPopup>();
    std::unique_ptr<CreateBidPopup> create_bid_pop_ = boost::make_unique<CreateBidPopup>();
    std::unique_ptr<ViewTransactionsPopup> viewtrans_pop_ = boost::make_unique<ViewTransactionsPopup>();
    std::unique_ptr<BidTransactionMadePopup> newtrans_pop_ = boost::make_unique<BidTransactionMadePopup>();
    s21::Client &client_;
};
#endif // SIMPLESTOCKEXCHANGE_VIEW_MAINWINDOW_HPP
