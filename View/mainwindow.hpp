#ifndef SIMPLESTOCKEXCHANGE_VIEW_MAINWINDOW_HPP
#define SIMPLESTOCKEXCHANGE_VIEW_MAINWINDOW_HPP

#include <memory.h>

#include <QCloseEvent>
#include <QMainWindow>
#include <boost/make_unique.hpp>

#include "../Client/Client.hpp"
#include "../Utility/ClientDisplayMessages.hpp"
#include "../Utility/ResponseParser.hpp"
#include "BidTransactionMadePopup.hpp"
#include "CreateBidPopup.hpp"
#include "LoginPopup.hpp"
#include "QuotationsPopup.hpp"
#include "RegisterPopup.hpp"
#include "UpdateBidPopup.hpp"
#include "UserSettings.hpp"
#include "ViewBids.hpp"
#include "ViewTransactionsPopup.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(s21::Client &client, QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void HandleLoginAttempt(const std::string username,
                          const std::string password);
  void HandleRegisterAttempt(const std::string username,
                             const std::string password,
                             const std::string balance_usd,
                             const std::string balance_rub);
  void HandleCreateBid(const std::string quantity, const std::string rate,
                       const std::string bid_type);
  void HandleViewBid(const std::string bid_type);
  void HandleUpdateBid(const std::string bid_id, const std::string bid_rate,
                       const std::string bid_quantity, int index);
  void HandleViewQuotations(const size_t time_period);

 protected:
  void closeEvent(QCloseEvent *event) override;

 private:
  bool WaitForServer();
  void SetLoginnedButtons();
  void SetNotLoginnedButtons();
  void Connect();
  void ConnectToHandlers();
  void ConnectToSettingsHandlers();
  void ConnectToPopups();
  void SetErrorText(const std::string &msg);
  Ui::MainWindow *ui;
  std::unique_ptr<LoginPopup> log_pop_ = boost::make_unique<LoginPopup>();
  std::unique_ptr<ViewBids> view_bid_pop_ = boost::make_unique<ViewBids>();
  std::unique_ptr<RegisterPopup> reg_pop_ = boost::make_unique<RegisterPopup>();
  std::unique_ptr<UserSettings> user_settings_ =
      boost::make_unique<UserSettings>();
  std::unique_ptr<QuotationsPopup> quot_pop_ =
      boost::make_unique<QuotationsPopup>();
  std::unique_ptr<UpdateBidPopup> upd_bid_pop_ =
      boost::make_unique<UpdateBidPopup>();
  std::unique_ptr<CreateBidPopup> create_bid_pop_ =
      boost::make_unique<CreateBidPopup>();
  std::unique_ptr<ViewTransactionsPopup> viewtrans_pop_ =
      boost::make_unique<ViewTransactionsPopup>();
  std::unique_ptr<BidTransactionMadePopup> newtrans_pop_ =
      boost::make_unique<BidTransactionMadePopup>();
  s21::Client &client_;
};
#endif  // SIMPLESTOCKEXCHANGE_VIEW_MAINWINDOW_HPP
