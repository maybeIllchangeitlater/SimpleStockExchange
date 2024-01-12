#ifndef SIMPLESTOCKEXCHANGE_CONTROLLERMAPPING_HPP
#define SIMPLESTOCKEXCHANGE_CONTROLLERMAPPING_HPP

#include <any>
#include <functional>
#include <unordered_map>

#include "../3rdParty/json.hpp"
#include "BidController.hpp"
#include "TransactionController.hpp"
#include "UserController.hpp"

namespace s21 {
struct ControllerMapping {
 public:
  using UserFunction =
      std::function<nlohmann::json(UserController&, const nlohmann::json&)>;
  using BidFunction =
      std::function<nlohmann::json(BidController&, const nlohmann::json&)>;
  using TransactionFunction = std::function<nlohmann::json(
      TransactionController&, const nlohmann::json&)>;
  static const std::unordered_map<std::string, UserFunction>
      method_mapping_user;
  static const std::unordered_map<std::string, BidFunction> method_mapping_bid;
  static const std::unordered_map<std::string, TransactionFunction>
      method_mapping_transaction;
};

}  // namespace s21

#endif  // SIMPLESTOCKEXCHANGE_CONTROLLERMAPPING_HPP
