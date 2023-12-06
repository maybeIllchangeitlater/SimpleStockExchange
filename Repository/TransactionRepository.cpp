#include "TransactionRepository.hpp"

namespace s21{
    void TransactionRepository::CreateTransaction(const std::string &id, const std::string &seller_id,
                                                  const std::string &buyer_id, const std::string &rate,
                                                  const std::string &quantity, const std::string &timestamp) {
        pqxx::work task(db_connection_);
        try{

            std::string sql = "INSERT INTO transaction_info (id, seller_id, buyer_id, rate, quantity) VALUES ("
                              + task.quote(id) + ", "
                              + task.quote(seller_id) + ", " + task.quote(buyer_id) + ", " + task.quote(rate) + ", "
                              + task.quote(quantity) + ", " + task.quote(timestamp) + ")";
            if(!task.exec(sql).empty()) {
                task.commit();
            }else{
                throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::ERROR));
            }
        }catch(...){
            task.abort();
            throw;
        }
    }

    pqxx::result TransactionRepository::ReadTransaction(const std::string &transaction_id) {
        pqxx::work task(db_connection_);
        try{

            std::string sql = "SELECT t.id AS transaction id, seller.name AS seller name, "
                              "buyer.name AS buyer name, t.rate, t.quantity, t.time FROM transaction_info t "
                              "JOIN user_info seller ON t.seller_id = seller.id "
                              "JOIN user_info buyer ON t.buyer_id = buyer.id "
                              "WHERE t.id = " + task.quote(transaction_id);
            pqxx::result res = task.exec(sql);
            if(!res.empty()) {
                return res;
            }else{
                throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::TRANSACTION_NOT_FOUND));
            }
        }catch(...){
            task.abort();
            throw;
        }
    }

    pqxx::result TransactionRepository::ReadAllUserSellTransactions(const std::string &user_id) {
        pqxx::work task(db_connection_);
        try{

            std::string sql = "SELECT t.id AS transaction id, seller.name AS seller name, "
                              "buyer.name AS buyer name, t.rate, t.quantity, t.time FROM transaction_info t "
                              "JOIN user_info seller ON t.seller_id = seller.id "
                              "JOIN user_info buyer ON t.buyer_id = buyer.id "
                              "WHERE t.seller_id = " + task.quote(user_id);
            pqxx::result res = task.exec(sql);
            if(!res.empty()) {
                return res;
            }else{
                throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::TRANSACTION_NOT_FOUND));
            }
        }catch(...){
            task.abort();
            throw;
        }
    }

    pqxx::result TransactionRepository::ReadAllUserBuyTransactions(const std::string &user_id) {
        pqxx::work task(db_connection_);
        try{

            std::string sql = "SELECT t.id AS transaction id, seller.name AS seller name, "
                              "buyer.name AS buyer name, t.rate, t.quantity, t.time FROM transaction_info t "
                              "JOIN user_info seller ON t.seller_id = seller.id "
                              "JOIN user_info buyer ON t.buyer_id = buyer.id "
                              "WHERE t.buyer_id = " + task.quote(user_id);
            pqxx::result res = task.exec(sql);
            if(!res.empty()) {
                return res;
            }else{
                throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::TRANSACTION_NOT_FOUND));
            }
        }catch(...){
            task.abort();
            throw;
        }
    }
}