#include "BidRepository.hpp"

namespace s21{

    void BidRepository::CreateSellBid(const std::string &seller_id, const std::string &rate,
                                      const std::string &quantity, const std::string &timestamp) {
            std::string sql = "INSERT INTO " + std::string(BDNames::bid_table)
                    + "(" + std::string(BDNames::bid_table_seller_id)
                    + ", " + std::string(BDNames::bid_table_rate)
                    + ", " + std::string(BDNames::bid_table_quantity)
                    + ", " + std::string(BDNames::bid_table_create_update_time)
                    + ") VALUES ('" + seller_id + "', '" + rate + "', '"
                    + quantity + "', '" + timestamp + "')";
            db_.ExecuteAndCommit(sql);
    }

    void BidRepository::CreateBuyBid(const std::string &buyer_id, const std::string &rate,
                                     const std::string &quantity, const std::string &timestamp) {
            std::string sql = "INSERT INTO " + std::string(BDNames::bid_table)
                              + "(" + std::string(BDNames::bid_table_buyer_id)
                              + ", " + std::string(BDNames::bid_table_rate)
                              + ", " + std::string(BDNames::bid_table_quantity)
                              + ", " + std::string(BDNames::bid_table_create_update_time)
                              + ") VALUES ('" + buyer_id
                              + "', '" + rate + "', '"
                              + quantity +  "', '" + timestamp + "')";
            db_.ExecuteAndCommit(sql);
    }

    pqxx::result BidRepository::ReadBid(const std::string &bid_id) {
            std::string sql = "SELECT b." + std::string(BDNames::bid_table_id)
                                + " AS " + BDNames::bid_id_for_join
                                + ", seller." + BDNames::user_table_user_name
                              + " AS " + BDNames::joined_seller_name
                              + ", buyer." + BDNames::user_table_user_name
                              + " AS " + BDNames::joined_buyer_name
                              + ", b." + BDNames::bid_table_rate
                              + ", b." + BDNames::bid_table_quantity
                              + ", b." + BDNames::bid_table_create_update_time
                              + " FROM " + BDNames::bid_table
                              + " b LEFT JOIN " + BDNames::user_table + " seller ON "
                              + BDNames::bid_table_seller_id
                              + " = seller." + BDNames::user_table_id
                              + " LEFT JOIN "  + BDNames::user_table + " buyer ON "
                              + BDNames::bid_table_buyer_id
                              + " = buyer." + BDNames::user_table_id
                              + " WHERE b." + BDNames::bid_table_id
                              + " = '" + bid_id + "'";
            return db_.Execute(sql);
    }

    pqxx::result BidRepository::ReadBidRaw(const std::string &bid_id) {
            std::string sql = "SELECT * FROM " + std::string(BDNames::bid_table)
                                                 + " WHERE " + std::string(BDNames::bid_table_id)
                                                 + " = '" + bid_id + "'";
            return db_.Execute(sql);
    }
    pqxx::result BidRepository::ReadAllUserSellBids(const std::string &seller_id) {
            std::string sql = "SELECT b." + std::string(BDNames::bid_table_id)
                            + " AS " + BDNames::bid_id_for_join
                            + ", seller." + std::string(BDNames::user_table_user_name)
                            + " AS " + BDNames::joined_seller_name
                              + ", buyer." + std::string(BDNames::user_table_user_name)
                              + " AS " + BDNames::joined_buyer_name
                              + ", b." + std::string(BDNames::bid_table_rate)
                              + ", b." + std::string(BDNames::bid_table_quantity)
                              + ", b." + std::string(BDNames::bid_table_create_update_time)
                              + " FROM " + std::string(BDNames::bid_table)
                              + " b JOIN " + std::string(BDNames::user_table) + " seller ON b."
                              + std::string(BDNames::bid_table_seller_id)
                              + " = seller." + std::string(BDNames::user_table_id)
                              + " LEFT JOIN "  + std::string(BDNames::user_table) + " buyer ON b."
                              + std::string(BDNames::bid_table_buyer_id)
                              + " = buyer." + std::string(BDNames::user_table_id)
                              + " WHERE seller." + std::string(BDNames::user_table_id)
                              + " = '" + seller_id + "'";
            return db_.Execute(sql);
    }

    pqxx::result BidRepository::ReadAllUserBuyBids(const std::string &buyer_id) {
            std::string sql = "SELECT b." + std::string(BDNames::bid_table_id)
                    + " AS " + BDNames::bid_id_for_join + ", seller."
                             + std::string(BDNames::user_table_user_name) +
                             " AS " + BDNames::joined_seller_name
                             + ", buyer." + std::string(BDNames::user_table_user_name)
                             + " AS " + BDNames::joined_buyer_name
                             + ", b." + std::string(BDNames::bid_table_rate)
                             + ", b." + std::string(BDNames::bid_table_quantity)
                             + ", b." + std::string(BDNames::bid_table_create_update_time)
                             + " FROM " + std::string(BDNames::bid_table)
                             + " b LEFT JOIN " + std::string(BDNames::user_table) + " seller ON b."
                             + std::string(BDNames::bid_table_seller_id)
                             + " = seller." + std::string(BDNames::user_table_id)
                             + " JOIN "  + std::string(BDNames::user_table) + " buyer ON b."
                             + std::string(BDNames::bid_table_buyer_id)
                             + " = buyer." + std::string(BDNames::user_table_id)
                              + " WHERE buyer." + std::string(BDNames::user_table_id)
                              + " = '" + buyer_id + "'";
            return db_.Execute(sql);
    }

    void BidRepository::DeleteBid(const std::string &bid_id) {
                std::string sql = "DELETE FROM " + std::string(BDNames::bid_table)
                                  + " WHERE " + std::string(BDNames::bid_table_id)
                                  + " = '" + bid_id + "'";
                db_.ExecuteAndCommit(sql);
    }



    void BidRepository::UpdateBidRate(const std::string &bid_id, const std::string &rate, const std::string &time)  {
            std::string sql = "UPDATE " + std::string(BDNames::bid_table)
                              + " SET " + std::string(BDNames::bid_table_rate)
                              + " = " + rate + ", time = '" + time
                              + "' WHERE " + std::string(BDNames::bid_table_id)
                              + " = '" + bid_id + "'";
            db_.ExecuteAndCommit(sql);
    }

    void BidRepository::UpdateBidQuantity(const std::string &bid_id, const std::string &quantity, const std::string &time) {
            std::string sql = "UPDATE " + std::string(BDNames::bid_table)
                              + " SET " + std::string(BDNames::bid_table_quantity)
                              + " = " + quantity + ", time = '" + time
                              + "' WHERE " + std::string(BDNames::bid_table_id)
                              + " = '" + bid_id + "'";
            db_.ExecuteAndCommit(sql);
    }

    pqxx::result BidRepository::MatchBuyBids(const std::string &user_id,
                                             const std::string &rate) {
        // usd for rubs
            std::string sql = "SELECT " + std::string(BDNames::bid_table_id)
                    + ", " + std::string(BDNames::bid_table_rate)
                    + ", " + std::string(BDNames::bid_table_buyer_id)
                    + ", " + std::string(BDNames::bid_table_quantity)
                    + " FROM " + std::string(BDNames::bid_table)
                    + " WHERE " + std::string(BDNames::bid_table_seller_id) + " is NULL AND "
                    + std::string(BDNames::bid_table_buyer_id) + " != '" + user_id
                    + "' AND " + std::string(BDNames::bid_table_rate) + " >= " + rate
                    + " ORDER BY " + std::string(BDNames::bid_table_rate) + " ASC";
            return db_.Execute(sql);
    }

    pqxx::result BidRepository::MatchSellBids(const std::string &user_id,
                                             const std::string &rate) {
        // rubs for USD
            std::string sql = "SELECT " + std::string(BDNames::bid_table_id)
                              + ", " + std::string(BDNames::bid_table_rate)
                              + ", " + std::string(BDNames::bid_table_seller_id)
                              + ", " + std::string(BDNames::bid_table_quantity)
                              + " FROM " + std::string(BDNames::bid_table)
                              + " WHERE " + std::string(BDNames::bid_table_buyer_id) + " is NULL AND "
                              + std::string(BDNames::bid_table_seller_id) + " != '" + user_id
                              + "' AND " + std::string(BDNames::bid_table_rate) + " <= " + rate
                              + " ORDER BY " + std::string(BDNames::bid_table_rate) + " DESC";
            return db_.Execute(sql);
    }

}