#ifndef SIMPLESTOCKEXCHANGE_CLIENT_HPP
#define SIMPLESTOCKEXCHANGE_CLIENT_HPP

#include <boost/asio.hpp>
#include <boost/make_unique.hpp>
#include <boost/shared_ptr.hpp>
#include <chrono>
#include <iostream>
#include <memory>

#include "../Connection/Connection.hpp"
#include "../Controller/ClientController.hpp"
#include "../Utility/ExtraJSONKeys.hpp"
#include "../Utility/ResponseParser.hpp"
#include "../Utility/ThreadSafeQ.hpp"

namespace s21 {
class Client {
  using connection_ptr = boost::shared_ptr<Connection>;
  using tcp = boost::asio::ip::tcp;

 public:
  Client() = default;

  bool Connect(const std::string &host, short port);
  void Disconnect();
  bool Connected();
  void Send(const std::string &message);

  ThreadSafeQ<std::pair<connection_ptr, std::string>> &Inbox() {
    return from_server_;
  }

  void Register(const std::string &username, const std::string &password,
                const std::string &balance_usd,
                const std::string &balance_rub) {
    Send(ClientController::Register(username, password, balance_usd,
                                    balance_rub));
  }

  void LogIn(const std::string &username, const std::string &password) {
    Send(ClientController::Login(username, password));
  }

  void CreateSellBid(const std::string &quantity, const std::string &rate) {
    Send(ClientController::CreateBid(GetUserId(), quantity, rate,
                                     BidService::SELLING));
  }

  void CreateBuyBid(const std::string &quantity, const std::string &rate) {
    Send(ClientController::CreateBid(GetUserId(), quantity, rate,
                                     BidService::BUYING));
  }

  void GetMySellBids() { Send(ClientController::GetMySellBids(GetUserId())); }

  void GetMyBuyBids() { Send(ClientController::GetMyBuyBids(GetUserId())); }

  void GetMySellTransactions() {
    Send(ClientController::GetMySellTransactions(GetUserId()));
  }

  void GetMyBuyTransactions() {
    Send(ClientController::GetMyBuyTransactions(GetUserId()));
  }

  void CancelBid(const std::string &bid_id) {
    Send(ClientController::CancelBid(bid_id, user_id_));
  }

  bool ChangeName(const std::string &user_name) {
    Send(ClientController::ChangeUserName(GetUserId(), user_name));
    while (from_server_.Empty()) {
    }
    return s21::ResponseParser::CheckStatus(from_server_.Front().second);
  }

  void ChangePassword(const std::string &new_password,
                      const std::string &old_password) {
    Send(ClientController::ChangePassword(GetUserId(), new_password,
                                          old_password));
  }

  bool DeleteMe(const std::string &user_password) {
    Send(ClientController::DeleteMe(GetUserId(), user_password));
    while (from_server_.Empty()) {
    }
    if (s21::ResponseParser::CheckStatus(from_server_.Front().second)) {
      Disconnect();
      return true;
    }
    return false;
  }

  void UpdateBidRate(const std::string &bid_id, const std::string &new_rate) {
    Send(ClientController::UpdateBidRate(bid_id, new_rate));
  }

  void UpdateBidQuantity(const std::string &bid_id,
                         const std::string &new_quantity) {
    Send(ClientController::UpdateBidQuantity(bid_id, new_quantity));
  }

  void CheckBalance() { Send(ClientController::GetMyBalance(user_id_)); }

  void CheckQuotations(size_t time_period) {
    Send(ClientController::GetQuotations(std::to_string(time_period)));
  }

  void WaitForResponse() {
    auto time = std::chrono::high_resolution_clock::now();
    while (from_server_.Empty()) {
      if (std::chrono::high_resolution_clock::now() - time >
          std::chrono::seconds(2)) {
        throw std::runtime_error("Lost connection to server");
      }
    }
  }

  void Authorize() {
    auto msg = from_server_.PopFront().second;
    size_t json_start = msg.find('{');
    size_t json_end = msg.rfind('}');
    std::string json_str = msg.substr(json_start, json_end - json_start + 1);
    nlohmann::json json = nlohmann::json::parse(json_str);
    user_id_ = json["id"];
  }

  void CutConnection() {
    context_->stop();
    if (thread_context_.joinable()) {
      thread_context_.join();
    }
    connection_->Disconnect();
    connection_.reset();
    context_.reset();
    from_server_.Clear();
  }

  const std::string &GetUserId() const { return user_id_; }

 private:
  std::unique_ptr<boost::asio::io_context> context_;
  boost::thread thread_context_;
  std::unique_ptr<Connection> connection_;
  ThreadSafeQ<std::pair<connection_ptr, std::string>> from_server_;
  std::string user_id_;
};
}  // namespace s21

#endif  // SIMPLESTOCKEXCHANGE_CLIENT_HPP
