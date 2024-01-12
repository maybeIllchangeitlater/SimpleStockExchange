#ifndef SIMPLESTOCKEXCHANGE_BALANCESERVICE_HPP
#define SIMPLESTOCKEXCHANGE_BALANCESERVICE_HPP

#include "../3rdParty/json.hpp"
#include "../Repository/BalanceRepository.hpp"
#include "../Repository/BdNames.hpp"
#include "../Service/BalanceServiceInterface.hpp"

namespace s21 {
class BalanceService : public BalanceServiceInterface {
 public:
  explicit BalanceService(BalanceRepository &balance_repository)
      : balance_repository_(balance_repository) {}

  ~BalanceService() override = default;

  void CreateUserBalance(const std::string &user_id,
                         const std::string &user_balance_usd,
                         const std::string &user_balance_rub) override {
    balance_repository_.CreateUserBalance(user_id, user_balance_usd,
                                          user_balance_rub);
  }

  nlohmann::json GetUserBalance(const std::string &user_id) override {
    nlohmann::json res;
    auto balance_info = balance_repository_.GetUserBalance(user_id);
    res[BDNames::balance_table_usd] =
        balance_info[0][BDNames::balance_table_usd].as<std::string>();
    res[BDNames::balance_table_rub] =
        balance_info[0][BDNames::balance_table_rub].as<std::string>();
    return res;
  }

  void SetUserBalance(const std::string &user_id,
                      const std::string &user_balance_usd,
                      const std::string &user_balance_rub) override {
    balance_repository_.SetUserBalance(user_id, user_balance_usd,
                                       user_balance_rub);
  }

  void IncreaseUsdDecreaseRubBalance(
      const std::string &user_id, const std::string &transaction_rate,
      const std::string &transaction_quantity) override {
    auto buyer_balance = balance_repository_.GetUserBalance(user_id);
    auto change_rub_balance_by =
        std::stod(transaction_rate) * std::stoll(transaction_quantity);
    SetUserBalance(
        user_id,
        std::to_string(
            buyer_balance[0][BDNames::balance_table_usd].as<double>() +
            std::stoll(transaction_quantity)),
        std::to_string(
            buyer_balance[0][BDNames::balance_table_rub].as<double>() -
            change_rub_balance_by));
  }

  void DecreaseUsdIncreaseRubBalance(
      const std::string &user_id, const std::string &transaction_rate,
      const std::string &transaction_quantity) override {
    auto seller_balance = balance_repository_.GetUserBalance(user_id);
    auto change_rub_balance_by =
        std::stod(transaction_rate) * std::stoll(transaction_quantity);
    SetUserBalance(
        user_id,
        std::to_string(
            seller_balance[0][BDNames::balance_table_usd].as<double>() -
            std::stoll(transaction_quantity)),
        std::to_string(
            seller_balance[0][BDNames::balance_table_rub].as<double>() +
            change_rub_balance_by));
  }

  bool ValidateBalance(const std::string &balance) override {
    try {
      std::stod(balance);
      return true;
    } catch (...) {
      return false;
    }
  }

 private:
  BalanceRepository &balance_repository_;
};
}  // namespace s21

#endif  // SIMPLESTOCKEXCHANGE_BALANCESERVICE_HPP
