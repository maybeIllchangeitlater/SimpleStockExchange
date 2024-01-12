#include "UserService.hpp"

namespace s21 {

void UserService::CreateUser(const std::string &user_name,
                             const std::string &password,
                             const std::string &user_balance_usd,
                             const std::string &user_balance_rub) {
  if (!ValidateUnique(user_name)) {
    throw std::logic_error(ServerMessage::server_message.at(
        ServerMessage::REGISTER_NOT_UNIQUE_NAME));
  }
  if (!ValidateUserName(user_name)) {
    throw std::logic_error(
        ServerMessage::server_message.at(ServerMessage::REGISTER_BAD_NAME));
  }
  if (!ValidatePassword(password)) {
    throw std::logic_error(
        ServerMessage::server_message.at(ServerMessage::REGISTER_BAD_PASSWORD));
  }
  if (!balance_service_.ValidateBalance(user_balance_usd) ||
      !balance_service_.ValidateBalance(user_balance_rub)) {
    throw std::logic_error(
        ServerMessage::server_message.at(ServerMessage::BALANCE_BAD_BALANCE));
  }
  repository_.CreateUser(user_name, Encoder::Encode(password));
  auto id = repository_.ReadUserByName(user_name)[0][BDNames::user_table_id]
                .as<std::string>();
  try {
    balance_service_.CreateUserBalance(id, user_balance_usd, user_balance_rub);
  } catch (...) {
    repository_.DeleteUser(id);
    throw;
  }
}

bool UserService::ValidatePassword(const std::string &password) {
  if (password.length() < 6) {
    return false;
  }
  bool has_uppercase = false;
  bool has_lowercase = false;
  bool has_symbols = false;
  for (const auto ch : password) {
    if (std::isupper(ch)) {
      has_uppercase = true;
    }
    if (std::islower(ch)) {
      has_lowercase = true;
    }
    if (!std::isalnum(ch) || std::isdigit(ch)) {
      has_symbols = true;
    }
  }
  return has_uppercase && has_lowercase && has_symbols;
}
}  // namespace s21