#include "TransactionRepository.hpp"

namespace s21{
    void TransactionRepository::CreateTransaction(const std::string &seller_id,
                                                  const std::string &buyer_id, const std::string &rate,
                                                  const std::string &quantity, const std::string &timestamp) {
            std::string sql = "INSERT INTO " + std::string(BDNames::transaction_table) + " ("
                    + BDNames::transaction_table_seller_id + ", "
                    + BDNames::transaction_table_buyer_id + ", "
                    + BDNames::transaction_table_rate + ", "
                    + BDNames::transaction_table_quantity + ", "
                    + BDNames::transaction_table_create_update_time
                    + ") VALUES ('" + seller_id + "', '" + buyer_id + "', '"
                    + rate + "', '" + quantity + "', '" + timestamp + "')";
            db_.ExecuteAndCommit(sql);
    }

    pqxx::result TransactionRepository::ReadTransaction(const std::string &transaction_id) {
            std::string sql = "SELECT t.'" + std::string(BDNames::transaction_table_id) + "' AS transaction id, "
                              + "seller.'" + BDNames::user_table_user_name + "' AS seller name, "
                              + "buyer.'" + BDNames::user_table_user_name + "' AS buyer name, "
                              + "t.'" + BDNames::transaction_table_rate
                              + "', t.'" + BDNames::transaction_table_quantity
                              + "', t.'" + BDNames::transaction_table_create_update_time
                              + "' FROM '" + BDNames::transaction_table + "' t "
                              + "JOIN '" + BDNames::user_table
                              + "' seller ON t.'" + BDNames::transaction_table_seller_id
                              + "' = seller.'" + BDNames::user_table_id
                              + "' JOIN'" + BDNames::user_table
                              + "' buyer ON t.'" + BDNames::transaction_table_buyer_id
                              + "' = buyer.'" + BDNames::user_table_id
                              + "' WHERE t.'" + BDNames::transaction_table_id
                              + "' = '" + transaction_id + "'";
            pqxx::result res = db_.Execute(sql);
            if(!res.empty()) {
                return res;
            }else{
                throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::TRANSACTION_NOT_FOUND));
            }
    }

    pqxx::result TransactionRepository::ReadAllUserSellTransactions(const std::string &user_id) {
            std::string sql = "SELECT b." + std::string(BDNames::transaction_table_id)
                              + " AS " + BDNames::transaction_id_for_join + ", seller."
                              + BDNames::user_table_user_name +
                              " AS " + BDNames::joined_seller_name
                              + ", buyer." + BDNames::user_table_user_name
                              + " AS " + BDNames::joined_buyer_name
                              + ", b." + BDNames::transaction_table_rate
                              + ", b." + BDNames::transaction_table_quantity
                              + ", b." + BDNames::transaction_table_create_update_time
                              + " FROM " + BDNames::transaction_table
                              + " b LEFT JOIN " + BDNames::user_table + " seller ON b."
                              + BDNames::bid_table_seller_id
                              + " = seller." + BDNames::user_table_id
                              + " JOIN "  + BDNames::user_table + " buyer ON b."
                              + BDNames::bid_table_buyer_id
                              + " = buyer." + BDNames::user_table_id
                              + " WHERE seller." + BDNames::user_table_id
                              + " = '" + user_id + "'";
            return db_.Execute(sql);
    }

    pqxx::result TransactionRepository::ReadAllUserBuyTransactions(const std::string &user_id) {
            std::string sql = "SELECT b." + std::string(BDNames::transaction_table_id)
                              + " AS " + BDNames::transaction_id_for_join
                              + ", seller." + BDNames::user_table_user_name
                              + " AS " + BDNames::joined_seller_name
                              + ", buyer." + BDNames::user_table_user_name
                              + " AS " + BDNames::joined_buyer_name
                              + ", b." + BDNames::transaction_table_rate
                              + ", b." + BDNames::transaction_table_quantity
                              + ", b." + BDNames::transaction_table_create_update_time
                              + " FROM " + BDNames::transaction_table
                              + " b JOIN " + BDNames::user_table + " seller ON b."
                              + BDNames::transaction_table_seller_id
                              + " = seller." + BDNames::user_table_id
                              + " LEFT JOIN "  + BDNames::user_table + " buyer ON b."
                              + BDNames::transaction_table_buyer_id
                              + " = buyer." + BDNames::user_table_id
                              + " WHERE buyer." + BDNames::user_table_id
                              + " = '" + user_id + "'";
            return db_.Execute(sql);
    }

    pqxx::result TransactionRepository::GetTransactionsForLast(const std::string &time_period) {
            std::string sql = "SELECT * FROM transaction_info WHERE " +
                    std::string(BDNames::transaction_table_create_update_time) +  " >= CURRENT_DATE - INTERVAL '" +
                    time_period + " days'";
            return db_.Execute(sql);
    }
}