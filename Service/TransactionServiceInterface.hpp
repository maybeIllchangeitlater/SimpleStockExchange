#ifndef SIMPLESTOCKEXCHANGE_TRANSACTIONSERVICEINTERFACE_HPP
#define SIMPLESTOCKEXCHANGE_TRANSACTIONSERVICEINTERFACE_HPP

#include <string>

#include "../3rdParty/json.hpp"

namespace s21 {
class TransactionServiceInterface {
 public:
  virtual ~TransactionServiceInterface() = default;
  virtual void MakeTransaction(const std::string &seller_id,
                               const std::string &buyer_id,
                               const std::string &rate,
                               const std::string &quantity) = 0;
  virtual nlohmann::json ReadTransaction(const std::string &transaction_id) = 0;
  virtual nlohmann::json ReadAllUserSellTransactions(
      const std::string &user_id) = 0;
  virtual nlohmann::json ReadAllUserBuyTransactions(
      const std::string &user_id) = 0;
  /// time period in days
  virtual nlohmann::json GetQuotations(const std::string &time_period) = 0;
};

}  // namespace s21
#endif  // SIMPLESTOCKEXCHANGE_TRANSACTIONSERVICEINTERFACE_HPP
