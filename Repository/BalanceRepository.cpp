#include "BalanceRepository.hpp"

namespace s21 {
void BalanceRepository::CreateUserBalance(const std::string &user_id,
                                          const std::string &balance_usd,
                                          const std::string &balance_rub) {
  pqxx::result res;
  std::string sql = "INSERT INTO " + std::string(BDNames::balance_table) +
                    " (" + BDNames::balance_table_user_id + ", " +
                    BDNames::balance_table_usd + ", " +
                    BDNames::balance_table_rub + ") VALUES ('" + user_id +
                    "', " + balance_usd + ", " + balance_rub + ");";
  db_.ExecuteAndCommit(sql);
}
pqxx::result BalanceRepository::GetUserBalance(const std::string &user_id) {
  pqxx::result res;
  std::string sql = "SELECT " + std::string(BDNames::balance_table_user_id) +
                    ", " + BDNames::balance_table_usd + ", " +
                    BDNames::balance_table_rub + " FROM " +
                    BDNames::balance_table + " WHERE " +
                    BDNames::balance_table_user_id + " = '" + user_id + "'";
  auto result = db_.Execute(sql);
  if (result.empty()) {
    throw std::runtime_error(
        ServerMessage::server_message.at(ServerMessage::USER_NOT_FOUND));
  }
  return result;
}

void BalanceRepository::SetUserBalance(const std::string &user_id,
                                       const std::string &balance_usd,
                                       const std::string &balance_rub) {
  std::string sql = "UPDATE " + std::string(BDNames::balance_table) + " SET " +
                    BDNames::balance_table_usd + " = '" + balance_usd + "', " +
                    BDNames::balance_table_rub + " = '" + balance_rub +
                    "' WHERE " + std::string(BDNames::balance_table_user_id) +
                    " = '" + user_id + "'";
  db_.ExecuteAndCommit(sql);
}
}  // namespace s21