#ifndef SIMPLESTOCKEXCHANGE_CONTROLLERS_CRUDCONTROLLER_HPP_
#define SIMPLESTOCKEXCHANGE_CONTROLLERS_CRUDCONTROLLER_HPP_

#include "../3rdParty/libpqxx/include/pqxx/pqxx"
#include <string>
#include "BdNames.hpp"
#include "../Utility/Encoder.hpp"
#include "../Utility/UUIDGenerator.hpp"
#include "../Utility/ServerMessage.hpp"

namespace s21 {

    class UserRepository {
    public:
        explicit UserRepository(pqxx::connection &db_conn) : db_connection_(db_conn) {}

        void CreateUser(const std::string &user_id, const std::string &username,
                                        const std::string &password, const std::string &user_balance);

        pqxx::result ReadUserById(const std::string &user_id);

        pqxx::result ReadUserByName(const std::string &user_name);

        void UpdateUserName(const std::string &user_id, const std::string &new_username);

        void UpdateUserPassword(const std::string &user_id, const std::string &new_password);

        void UpdateUserBalance(const std::string &user_id, const std::string &new_balance);

        void DeleteUser(const std::string &user_id);



    private:
        pqxx::connection &db_connection_;
    };

} // s21

#endif //SIMPLESTOCKEXCHANGE_CONTROLLERS_CRUDCONTROLLER_HPP_
