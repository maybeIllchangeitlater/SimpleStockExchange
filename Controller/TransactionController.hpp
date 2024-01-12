#ifndef SIMPLESTOCKEXCHANGE_TRANSACTIONCONTROLLER_HPP
#define SIMPLESTOCKEXCHANGE_TRANSACTIONCONTROLLER_HPP

#include <vector>

#include "../3rdParty/json.hpp"
#include "../Repository/BdNames.hpp"
#include "../Service/TransactionServiceInterface.hpp"
#include "../Utility/ExtraJSONKeys.hpp"
#include "../Utility/ResponseError.hpp"

namespace s21 {
class TransactionController {
 public:
  explicit TransactionController(TransactionServiceInterface &service)
      : service_(service) {}

  nlohmann::json GetUserSellTransactions(const nlohmann::json &request_body);

  nlohmann::json GetUserBuyTransactions(const nlohmann::json &request_body);

  nlohmann::json GetQuotations(const nlohmann::json &request_body);

 private:
  TransactionServiceInterface &service_;
};
}  // namespace s21

#endif  // SIMPLESTOCKEXCHANGE_TRANSACTIONCONTROLLER_HPP
// pack / unpuck to http to request/responce class

// cascade delete in

/// lock to service