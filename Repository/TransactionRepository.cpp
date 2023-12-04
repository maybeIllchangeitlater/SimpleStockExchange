#include "TransactionRepository.hpp"

namespace s21{

    void TransactionRepository::CreateTransaction(const std::string &post_user_id, const std::string &rate,
                                                  const std::string &quantity) {
        pqxx::work task(db_connection_);
        try{

            std::string sql = "INSERT INTO transaction_info (id, post_user_id, rate, quantity) VALUES ("
                              + task.quote(UUIDGenerator::Generate()) + ", "
                              + task.quote(post_user_id) + ", " + task.quote(rate) + ", "
                              + task.quote(quantity) + ")";
            task.exec(sql);
            task.commit();
        }catch(...){
            task.abort();
            //add err msg?
        }
    }

    void TransactionRepository::ReadTransaction(const std::string &transaction_id) {
        pqxx::work task(db_connection_);
        try{

            std::string sql = "SELECT t.id AS transaction_id, post_user.name AS post username,"
                              "buy_user.name AS buy username, t.rate, t.quantity FROM transaction_info t "
                              "JOIN user_info post_user ON t.post_user_id = post_user.id "
                              "JOIN user_info buy_user ON t.buy_user_id = buy_user.id "
                              "WHERE id = " + task.quote(transaction_id);
            task.exec(sql);
            task.commit();
        }catch(...){
            task.abort();
            //add err msg?
        }
    }

    void TransactionRepository::ReadAllUserSellTransactions(const std::string &user_id) {
        pqxx::work task(db_connection_);
        try{

            std::string sql = "SELECT t.id AS transaction_id, post_user.name AS post username,"
                              "buy_user.name AS buy username, t.rate, t.quantity FROM transaction_info t "
                              "JOIN user_info post_user ON t.post_user_id = post_user.id "
                              "JOIN user_info buy_user ON t.buy_user_id = buy_user.id "
                              "WHERE post_user.id = " + task.quote(user_id);
            task.exec(sql);
            task.commit();
        }catch(...){
            task.abort();
            //add err msg?
        }
    }

    void TransactionRepository::ReadAllUserBuyTransactions(const std::string &user_id) {
        pqxx::work task(db_connection_);
        try{

            std::string sql = "SELECT t.id AS transaction_id, post_user.name AS post username,"
                              "buy_user.name AS buy username, t.rate, t.quantity FROM transaction_info t "
                              "JOIN user_info post_user ON t.post_user_id = post_user.id "
                              "JOIN user_info buy_user ON t.buy_user_id = buy_user.id "
                              "WHERE buy_user.id = " + task.quote(user_id);
            task.exec(sql);
            task.commit();
        }catch(...){
            task.abort();
            //add err msg?
        }
    }

    void TransactionRepository::DeleteTransaction(const std::string &transaction_id) {
        pqxx::work task(db_connection_);
        try{
            std::string buyer = "SELECT buy_user_id FROM transaction_info WHERE id = " + task.quote(transaction_id);
            pqxx::result result = task.exec(buyer);
            if (result.empty() || result[0]["buy_user_id"].is_null()) {
                std::string sql = "DELETE FROM transaction_info WHERE id = " + task.quote(transaction_id);
                task.exec(sql);
                task.commit();
            } else {
            throw std::logic_error("Cannot delete finished transaction");
        }
    } catch (...){
            task.abort();
        }

    }

    void TransactionRepository::UpdateTransactionRate(const std::string &transaction_id, const std::string &rate) {
        pqxx::work task(db_connection_);
        try{
            std::string sql = "UPDATE transaction_info SET rate = " + task.quote(rate) + " WHERE id = "
                              + task.quote(transaction_id);
            task.exec(sql);
            task.commit();
        }catch(...){
            task.abort();
        }
    }

    void TransactionRepository::UpdateTransactionQuantity(const std::string &transaction_id,
                                                          const std::string &quantity) {
        pqxx::work task(db_connection_);
        try{
            std::string sql = "UPDATE transaction_info SET quantity = " + task.quote(quantity) + " WHERE id = "
                              + task.quote(transaction_id);
            task.exec(sql);
            task.commit();
        }catch(...){
            task.abort();
        }
    }

    void TransactionRepository::UpdateTransactionBuyer(const std::string &transaction_id,
                                                       const std::string &buy_user_id) {
        pqxx::work task(db_connection_);
        try{
            std::string sql = "UPDATE transaction_info SET buy_user_id = " + task.quote(buy_user_id) + " WHERE id = "
                              + task.quote(transaction_id);
            task.exec(sql);
            task.commit();
        }catch(...){
            task.abort();
        }
    }
}