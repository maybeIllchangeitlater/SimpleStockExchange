#ifndef SIMPLESTOCKEXCHANGE_CONTROLLERS_CRUDCONTROLLER_HPP_
#define SIMPLESTOCKEXCHANGE_CONTROLLERS_CRUDCONTROLLER_HPP_

#include <string>

#include "../3rdParty/libpqxx/include/pqxx/pqxx"
#include "../Utility/Encoder.hpp"
#include "../Utility/ServerMessage.hpp"
#include "BdNames.hpp"
#include "DatabaseInterface.hpp"

namespace s21 {

class UserRepository {
 public:
  explicit UserRepository(DatabaseInterface &db) : db_(db) {}

  void CreateUser(const std::string &username, const std::string &password);

  pqxx::result ReadUserById(const std::string &user_id);

  pqxx::result ReadUserByName(const std::string &username);

  void UpdateUserName(const std::string &user_id,
                      const std::string &new_username);

  void UpdateUserPassword(const std::string &user_id,
                          const std::string &new_password);

  void DeleteUser(const std::string &user_id);

 private:
  DatabaseInterface &db_;
};

}  // namespace s21

#endif  // SIMPLESTOCKEXCHANGE_CONTROLLERS_CRUDCONTROLLER_HPP_
