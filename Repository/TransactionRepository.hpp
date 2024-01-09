#ifndef SIMPLESTOCKEXCHANGE_TRANSACTIONREPOSITORY_HPP
#define SIMPLESTOCKEXCHANGE_TRANSACTIONREPOSITORY_HPP

#include "../3rdParty/libpqxx/include/pqxx/pqxx"
#include "BdNames.hpp"
#include "../3rdParty/json.hpp"
#include "../Utility/ServerMessage.hpp"
#include "DatabaseInterface.hpp"
#include <iostream>

namespace s21 {
    class TransactionRepository {
    public:
        explicit TransactionRepository(DatabaseInterface &db) : db_(db) {}
        void CreateTransaction(const std::string &seller_id, const std::string &buyer_id,
                               const std::string &rate, const std::string &quantity, const std::string &timestamp);
        pqxx::result ReadTransaction(const std::string &id);
        pqxx::result ReadAllUserSellTransactions(const std::string &user_id);
        pqxx::result ReadAllUserBuyTransactions(const std::string &user_id);
        pqxx::result GetTransactionsForLast(const std::string &time_period);
    private:
        DatabaseInterface &db_;
    };
} //s21

#endif //SIMPLESTOCKEXCHANGE_TRANSACTIONREPOSITORY_HPP
