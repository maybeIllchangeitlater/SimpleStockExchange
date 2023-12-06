#include "TransactionRepository.hpp"

namespace s21{
    void TransactionRepository::CreateTransaction(const std::string &id, const std::string &seller_id,
                                                  const std::string &buyer_id, const std::string &rate,
                                                  const std::string &quantity, const std::string &timestamp) {
        pqxx::work task(db_connection_);
        try{

            std::string sql = "INSERT INTO " + task.quote(BDNames::transaction_table) + " ("
                    + task.quote(BDNames::transaction_table_id) + ", "
                    + task.quote(BDNames::transaction_table_seller_id) + ", "
                    + task.quote(BDNames::transaction_table_buyer_id) + ", "
                    + task.quote(BDNames::transaction_table_rate) + ", "
                    + task.quote(BDNames::transaction_table_quantity)
                    + " VALUES (" + task.quote(id) + ", "
                    + task.quote(seller_id) + ", " + task.quote(buyer_id) + ", "
                    + task.quote(rate) + ", " + task.quote(quantity) + ", " + task.quote(timestamp) + ")";
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

            std::string sql = "SELECT t." + task.quote(BDNames::transaction_table_id) + " AS transaction id, "
                              + "seller." + task.quote(BDNames::user_table_user_name) + " AS seller name, "
                              + "buyer." + task.quote(BDNames::user_table_user_name) + " AS buyer name, "
                              + "t." + task.quote(BDNames::transaction_table_rate)
                              + ", t." + task.quote(BDNames::transaction_table_quantity)
                              + ", t." + task.quote(BDNames::transaction_table_create_update_time)
                              + " FROM " + task.quote(BDNames::transaction_table) + " t "
                              + "JOIN " + task.quote(BDNames::user_table)
                              + " seller ON t." + task.quote(BDNames::transaction_table_seller_id)
                              + " = seller." + task.quote(BDNames::user_table_id)
                              + " JOIN" + task.quote(BDNames::user_table)
                              + " buyer ON t." + task.quote(BDNames::transaction_table_buyer_id)
                              + " = buyer." + task.quote(BDNames::user_table_id)
                              + " WHERE t." + task.quote(BDNames::transaction_table_id)
                              + " = " + task.quote(transaction_id);
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

            std::string sql = "SELECT t." + task.quote(BDNames::transaction_table_id) + " AS transaction id, "
                              + "seller." + task.quote(BDNames::user_table_user_name) + " AS seller name, "
                              + "buyer." + task.quote(BDNames::user_table_user_name) + " AS buyer name, "
                              + "t." + task.quote(BDNames::transaction_table_rate)
                              + ", t." + task.quote(BDNames::transaction_table_quantity)
                              + ", t." + task.quote(BDNames::transaction_table_create_update_time)
                              + " FROM " + task.quote(BDNames::transaction_table) + " t "
                              + "JOIN " + task.quote(BDNames::user_table)
                              + " seller ON t." + task.quote(BDNames::transaction_table_seller_id)
                              + " = seller." + task.quote(BDNames::user_table_id)
                              + " JOIN" + task.quote(BDNames::user_table)
                              + " buyer ON t." + task.quote(BDNames::transaction_table_buyer_id)
                              + " = buyer." + task.quote(BDNames::user_table_id)
                              + " WHERE seller." + task.quote(BDNames::user_table_id)
                              + " = " + task.quote(user_id);
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

            std::string sql = "SELECT t." + task.quote(BDNames::transaction_table_id) + " AS transaction id, "
                              + "seller." + task.quote(BDNames::user_table_user_name) + " AS seller name, "
                              + "buyer." + task.quote(BDNames::user_table_user_name) + " AS buyer name, "
                              + "t." + task.quote(BDNames::transaction_table_rate)
                              + ", t." + task.quote(BDNames::transaction_table_quantity)
                              + ", t." + task.quote(BDNames::transaction_table_create_update_time)
                              + " FROM " + task.quote(BDNames::transaction_table) + " t "
                              + "JOIN " + task.quote(BDNames::user_table)
                              + " seller ON t." + task.quote(BDNames::transaction_table_seller_id)
                              + " = seller." + task.quote(BDNames::user_table_id)
                              + " JOIN" + task.quote(BDNames::user_table)
                              + " buyer ON t." + task.quote(BDNames::transaction_table_buyer_id)
                              + " = buyer." + task.quote(BDNames::user_table_id)
                              + " WHERE buyer." + task.quote(BDNames::user_table_id)
                              + " = " + task.quote(user_id);
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