#ifndef SIMPLESTOCKEXCHANGE_TRANSACTIONREPOSITORY_HPP
#define SIMPLESTOCKEXCHANGE_TRANSACTIONREPOSITORY_HPP

#include <pqxx/pqxx>
#include "../3rdParty/json.hpp"

namespace s21 {
    class TransactionRepository {
    public:
        explicit TransactionRepository(pqxx::connection &db_conn) : db_connection_(db_conn) {}
        void CreateTransaction(const std::string &id, const std::string &seller_id, const std::string &buyer_id,
                               const std::string &rate, const std::string &quantity, const std::string &timestamp);
        pqxx::result ReadTransaction(const std::string &id);
        pqxx::result ReadAllUserSellTransactions(const std::string &user_id);
        pqxx::result ReadAllUserBuyTransactions(const std::string &user_id);
    private:
        pqxx::connection &db_connection_;
    };
} //s21

#endif //SIMPLESTOCKEXCHANGE_TRANSACTIONREPOSITORY_HPP
