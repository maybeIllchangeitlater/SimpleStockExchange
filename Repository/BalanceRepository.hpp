#ifndef SIMPLESTOCKEXCHANGE_BALANCEREPOSITORY_HPP
#define SIMPLESTOCKEXCHANGE_BALANCEREPOSITORY_HPP

#include "../3rdParty/libpqxx/include/pqxx/pqxx"
#include "BdNames.hpp"
#include "../Utility/ServerMessage.hpp"
#include <iostream>

namespace s21 {
    class BalanceRepository {
    public:
        explicit BalanceRepository(pqxx::connection &db_connection) : db_connection_(db_connection){}
        void CreateUserBalance(const std::string &user_id, const std::string &balance_usd,
                               const std::string &balance_rub);
        pqxx::result GetUserBalance(const std::string &user_id);
        void SetUserBalance(const std::string &user_id, const std::string &balance_usd,
                            const std::string &balance_rub);

    private:
        pqxx::connection &db_connection_;
    };
}

#endif //SIMPLESTOCKEXCHANGE_BALANCEREPOSITORY_HPP
