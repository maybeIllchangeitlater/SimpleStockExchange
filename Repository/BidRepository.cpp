#include "BidRepository.hpp"
#include <iostream>

namespace s21{

    void BidRepository::CreateSellBid(const std::string &seller_id, const std::string &rate,
                                      const std::string &quantity, const std::string &timestamp) {
        pqxx::work task(db_connection_);
        std::cout << "trying to create sell bid\n";
        try{
            std::string sql = "INSERT INTO " + std::string(BDNames::bid_table)
                    + "(" + std::string(BDNames::bid_table_seller_id)
                    + ", " + std::string(BDNames::bid_table_rate)
                    + ", " + std::string(BDNames::bid_table_quantity)
                    + ", " + std::string(BDNames::bid_table_create_update_time)
                    + ") VALUES (" + task.quote(seller_id) + ", " + task.quote(rate) + ", "
                    + task.quote(quantity) + ", " + task.quote(timestamp) + ")";
            std::cout << "\nSQL REQUEST :\n" << sql;
            auto result = task.exec(sql);
            task.commit();
        }catch(const std::exception &e){
            std::cout << "\naboorting task because " << e.what() << "\n";
            task.abort();
            throw;
        }
    }

    void BidRepository::CreateBuyBid(const std::string &buyer_id, const std::string &rate,
                                     const std::string &quantity, const std::string &timestamp) {
        pqxx::work task(db_connection_);
        try{
            std::string sql = "INSERT INTO " + std::string(BDNames::bid_table)
                              + "(" + std::string(BDNames::bid_table_buyer_id)
                              + ", " + std::string(BDNames::bid_table_rate)
                              + ", " + std::string(BDNames::bid_table_quantity)
                              + ", " + std::string(BDNames::bid_table_create_update_time)
                              + ") VALUES (" + task.quote(buyer_id)
                              + ", " + task.quote(rate) + ", "
                              + task.quote(quantity) +  ", " + task.quote(timestamp) + ")";
            std::cout << "\nSQL REQUEST :\n" << sql;
            auto result = task.exec(sql);
            task.commit();
        }catch(const std::exception &e){
            std::cout << "\naboorting task because " << e.what() << "\n";
            task.abort();
            throw;
        }
    }

    pqxx::result BidRepository::ReadBid(const std::string &bid_id) {
        pqxx::work task(db_connection_);
        try{
            std::string sql = "SELECT b." + task.quote(BDNames::bid_table_id)
                                + " AS " + BDNames::bid_id_for_join
                                + ", seller." + task.quote(BDNames::user_table_user_name)
                              + " AS " + BDNames::joined_seller_name
                              + ", buyer." + task.quote(BDNames::user_table_user_name)
                              + " AS " + BDNames::joined_buyer_name
                              + ", b." + task.quote(BDNames::bid_table_rate)
                              + " b." + task.quote(BDNames::bid_table_quantity)
                              + " b." + task.quote(BDNames::bid_table_create_update_time)
                              + " FROM " + task.quote(BDNames::bid_table)
                              + " b LEFT JOIN " + task.quote(BDNames::user_table) + " seller ON b."
                              + task.quote(BDNames::bid_table_seller_id)
                              + " = seller." + task.quote(BDNames::user_table_id)
                              + " LEFT JOIN "  + task.quote(BDNames::user_table) + " buyer ON b."
                              + task.quote(BDNames::bid_table_buyer_id)
                              + " = buyer." + task.quote(BDNames::user_table_id)
                              + " WHERE " + task.quote(BDNames::bid_table_id)
                              + " = " + task.quote(bid_id);
            std::cout << sql << "\n";
            auto res = task.exec(sql);
            return res;
        }catch(const std::exception &e){
            std::cout << "\naboorting task because " << e.what() << "\n";
            task.abort();
            throw;
        }
    }

    pqxx::result BidRepository::ReadBidRaw(const std::string &bid_id) {
        pqxx::work task(db_connection_);
        try{
            std::string sql = "SELECT * FROM " + std::string(BDNames::bid_table)
                                                 + " WHERE " + std::string(BDNames::bid_table_id)
                                                 + " = " + task.quote(bid_id);
            std::cout << sql << "\n";
            auto res = task.exec(sql);
            return res;
        }catch(const std::exception &e){
            std::cout << "\naboorting task because " << e.what() << "\n";
            task.abort();
            throw;
        }
    }
    pqxx::result BidRepository::ReadAllUserSellBids(const std::string &seller_id) {
        pqxx::work task(db_connection_);
        try{
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
                              + " = " + task.quote(seller_id);
            std::cout << sql << "\n";
            auto res = task.exec(sql);
            return res;
        }catch(const std::exception &e){
            std::cout << "\naboorting task because " << e.what() << "\n";
            task.abort();
            throw;
        }
    }

    pqxx::result BidRepository::ReadAllUserBuyBids(const std::string &buyer_id) {
        pqxx::work task(db_connection_);
        try{

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
                              + " = " + task.quote(buyer_id);
            std::cout << sql << "\n";
            auto res = task.exec(sql);
            return res;
        }catch(const std::exception &e){
            std::cout << "\naboorting task because " << e.what() << "\n";
            task.abort();
            throw;
        }
    }

    void BidRepository::DeleteBid(const std::string &bid_id) {
        pqxx::work task(db_connection_);
        try{
                std::string sql = "DELETE FROM " + std::string(BDNames::bid_table)
                                  + " WHERE " + std::string(BDNames::bid_table_id)
                                  + " = " + task.quote(bid_id);
                std::cout << sql << "\n";
                auto result = task.exec(sql);
                task.commit();
        } catch (const std::exception &e){
            std::cout << "\naboorting task because " << e.what() << "\n";
            task.abort();
            throw;
        }
    }



    void BidRepository::UpdateBidRate(const std::string &bid_id, const std::string &rate, const std::string &time)  {
        pqxx::work task(db_connection_);
        try{
            std::string sql = "UPDATE " + std::string(BDNames::bid_table)
                              + " SET " + std::string(BDNames::bid_table_rate)
                              + " = " + rate + ", time = " + task.quote(time)
                              + " WHERE " + std::string(BDNames::bid_table_id)
                              + " = " + task.quote(bid_id);
            std::cout << sql << "\n";
            task.exec(sql);
            task.commit();
        }catch(const std::exception &e){
            std::cout << "\naboorting task because " << e.what() << "\n";
            task.abort();
            throw;
        }
    }

    void BidRepository::UpdateBidQuantity(const std::string &bid_id, const std::string &quantity, const std::string &time) {
        pqxx::work task(db_connection_);
        try{
            std::string sql = "UPDATE " + std::string(BDNames::bid_table)
                              + " SET " + std::string(BDNames::bid_table_quantity)
                              + " = " + quantity + ", time = " + task.quote(time)
                              + " WHERE " + std::string(BDNames::bid_table_id)
                              + " = " + task.quote(bid_id);
            std::cout << sql << "\n";
            task.exec(sql);
            task.commit();
        }catch(const std::exception &e){
            std::cout << "\naboorting task because " << e.what() << "\n";
            task.abort();
            throw;
        }
    }

}