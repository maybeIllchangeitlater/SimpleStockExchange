#ifndef SIMPLESTOCKEXCHANGE_BALANCEREPOSITORY_HPP
#define SIMPLESTOCKEXCHANGE_BALANCEREPOSITORY_HPP

#include "../3rdParty/libpqxx/include/pqxx/pqxx"
#include "../Utility/ServerMessage.hpp"
#include "BdNames.hpp"
#include "DatabaseInterface.hpp"

namespace s21 {
class BalanceRepository {
 public:
  explicit BalanceRepository(DatabaseInterface &db) : db_(db) {}
  void CreateUserBalance(const std::string &user_id,
                         const std::string &balance_usd,
                         const std::string &balance_rub);
  pqxx::result GetUserBalance(const std::string &user_id);
  void SetUserBalance(const std::string &user_id,
                      const std::string &balance_usd,
                      const std::string &balance_rub);

 private:
  DatabaseInterface &db_;
};
}  // namespace s21

#endif  // SIMPLESTOCKEXCHANGE_BALANCEREPOSITORY_HPP
