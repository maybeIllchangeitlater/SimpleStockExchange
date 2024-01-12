#ifndef SIMPLESTOCKEXCHANGE_USERSERVICEINTERFACE_HPP
#define SIMPLESTOCKEXCHANGE_USERSERVICEINTERFACE_HPP

#include <string>

#include "../3rdParty/json.hpp"

namespace s21 {
class UserServiceInterface {
 public:
  virtual ~UserServiceInterface() = default;
  virtual void CreateUser(const std::string &user_name,
                          const std::string &password,
                          const std::string &user_balance_usd,
                          const std::string &user_balance_rub) = 0;
  virtual nlohmann::json GetUserByName(const std::string &user_name) = 0;
  virtual nlohmann::json GetUserById(const std::string &user_id) = 0;
  virtual void UpdateUserName(const std::string &user_id,
                              const std::string &new_name) = 0;
  virtual void UpdateUserBalance(const std::string &user_id,
                                 const std::string &new_balance_usd,
                                 const std::string &new_balance_rub) = 0;
  virtual void UpdateUserPassword(const std::string &user_id,
                                  const std::string &new_password,
                                  const std::string &old_password) = 0;
  virtual void DeleteUser(const std::string &user_id,
                          const std::string &user_password) = 0;
};
}  // namespace s21

#endif  // SIMPLESTOCKEXCHANGE_USERSERVICEINTERFACE_HPP
