#ifndef SIMPLESTOCKEXCHANGE_TRANSACTIONCRUDCONTROLLER_HPP
#define SIMPLESTOCKEXCHANGE_TRANSACTIONCRUDCONTROLLER_HPP

#include <pqxx/pqxx>
#include "../Utility/UUIDGenerator.hpp"

namespace s21 {
    class TransactionCRUDController {
    public:
        explicit TransactionCRUDController(pqxx::connection &db_conn) :db_connection_(db_conn) {}

        void CreateTransaction(const std::string &post_user_id, const std::string& rate,
                               const std::string &quantity);

        void ReadTransaction(const std::string &transaction_id);
        void ReadAllUserSellTransactions(const std::string &user_id);
        void ReadAllUserBuyTransactions(const std::string &user_id);
//        void ReadQuotations(){
//
//        }
        void UpdateTransactionRate(const std::string &transaction_id, const std::string &rate);
        void UpdateTransactionQuantity(const std::string &transaction_id, const std::string &quantity);
        void UpdateTransactionBuyer(const std::string &transaction_id, const std::string &buy_user_id);
        void DeleteTransaction(const std::string &transaction_id);
    private:
        pqxx::connection &db_connection_;
    };
} //s21

#endif //SIMPLESTOCKEXCHANGE_TRANSACTIONCRUDCONTROLLER_HPP
