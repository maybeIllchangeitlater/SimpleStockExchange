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
                    + " VALUE (" + seller_id + ", " + rate + ", "
                    + quantity + ", " + timestamp + ")";
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
            std::string sql = "INSERT INTO " + task.quote(BDNames::bid_table)
                              + "(" + task.quote(BDNames::bid_table_buyer_id)
                              + ", " + task.quote(BDNames::bid_table_rate)
                              + ", " + task.quote(BDNames::bid_table_quantity)
                              + ", " + task.quote(BDNames::bid_table_create_update_time)
                              + " VALUE (" + task.quote(buyer_id)
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

            std::string sql = "SELECT b." + task.quote(BDNames::bid_table_id) + " AS bid id, seller."
                              + task.quote(BDNames::user_table_user_name) + " AS seller username, buyer."
                              + task.quote(BDNames::user_table_user_name) + " AS buyer username, b."
                              + task.quote(BDNames::bid_table_rate)
                              + " b." + task.quote(BDNames::bid_table_quantity)
                              + " b." + task.quote(BDNames::bid_table_create_update_time)
                              + " FROM " + task.quote(BDNames::bid_table)
                              + " b JOIN " + task.quote(BDNames::user_table) + " seller ON b."
                              + task.quote(BDNames::bid_table_seller_id)
                              + " = seller." + task.quote(BDNames::user_table_id)
                              + " JOIN "  + task.quote(BDNames::user_table) + " buyer ON b."
                              + task.quote(BDNames::bid_table_buyer_id)
                              + " = buyer." + task.quote(BDNames::user_table_id)
                              + " WHERE " + task.quote(BDNames::bid_table_id)
                              + " = " + task.quote(bid_id);
            pqxx::result res = task.exec(sql);
            if(!res.empty()) {
                return res;
            }else{
                throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::BID_NOT_FOUND));
            }
        }catch(...){
            task.abort();
            throw;
        }
    }

    pqxx::result BidRepository::ReadAllUserSellBids(const std::string &seller_id) {
        pqxx::work task(db_connection_);
        try{

            std::string sql = "SELECT b." + task.quote(BDNames::bid_table_id) + " AS bid id, seller."
                              + task.quote(BDNames::user_table_user_name) + " AS seller username, buyer."
                              + task.quote(BDNames::user_table_user_name) + " AS buyer username, b."
                              + task.quote(BDNames::bid_table_rate)
                              + " b." + task.quote(BDNames::bid_table_quantity)
                              + " b." + task.quote(BDNames::bid_table_create_update_time)
                              + " FROM " + task.quote(BDNames::bid_table)
                              + " b JOIN " + task.quote(BDNames::user_table) + " seller ON b."
                              + task.quote(BDNames::bid_table_seller_id)
                              + " = seller." + task.quote(BDNames::user_table_id)
                              + " JOIN "  + task.quote(BDNames::user_table) + " buyer ON b."
                              + task.quote(BDNames::bid_table_buyer_id)
                              + " = buyer." + task.quote(BDNames::user_table_id)
                              + " WHERE seller." + task.quote(BDNames::user_table_id)
                              + " = " + task.quote(seller_id);
            pqxx::result res = task.exec(sql);
            if(!res.empty()) {
                return res;
            }else{
                throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::BID_NOT_FOUND));
            }
        }catch(...){
            task.abort();
            throw;
        }
    }

    pqxx::result BidRepository::ReadAllUserBuyBids(const std::string &buyer_id) {
        pqxx::work task(db_connection_);
        try{

            std::string sql = "SELECT b." + task.quote(BDNames::bid_table_id) + " AS bid id, seller."
                              + task.quote(BDNames::user_table_user_name) + " AS seller username, buyer."
                              + task.quote(BDNames::user_table_user_name) + " AS buyer username, b."
                              + task.quote(BDNames::bid_table_rate)
                              + " b." + task.quote(BDNames::bid_table_quantity)
                              + " b." + task.quote(BDNames::bid_table_create_update_time)
                              + " FROM " + task.quote(BDNames::bid_table)
                              + " b JOIN " + task.quote(BDNames::user_table) + " seller ON b."
                              + task.quote(BDNames::bid_table_seller_id)
                              + " = seller." + task.quote(BDNames::user_table_id)
                              + " JOIN "  + task.quote(BDNames::user_table) + " buyer ON b."
                              + task.quote(BDNames::bid_table_buyer_id)
                              + " = buyer." + task.quote(BDNames::user_table_id)
                              + " WHERE buyer." + task.quote(BDNames::user_table_id)
                              + " = " + task.quote(buyer_id);
            pqxx::result res = task.exec(sql);
            if(!res.empty()) {
                return res;
            }else{
                throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::BID_NOT_FOUND));
            }
        }catch(...){
            task.abort();
            throw;
        }
    }

    void BidRepository::DeleteUnfinishedBid(const std::string &bid_id) {
        pqxx::work task(db_connection_);
        try{
            std::string participants = "SELECT " + task.quote(BDNames::bid_table_buyer_id) + ", "
                                + task.quote(BDNames::bid_table_seller_id) + ", "
                                + " FROM " + task.quote(BDNames::bid_table)
                                + " WHERE " + task.quote(BDNames::bid_table_id)
                                + " = " + task.quote(bid_id);
            pqxx::result result = task.exec(participants);
            if(result.empty()){
                throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::BID_NOT_FOUND));
            }
            if (result[0][BDNames::bid_table_seller_id].is_null()
                || result[0][BDNames::bid_table_buyer_id].is_null()) {
                std::string sql = "DELETE FROM " + task.quote(BDNames::bid_table)
                        + " WHERE " + task.quote(BDNames::bid_table_id)
                        + " = " + task.quote(bid_id);
                if(!task.exec(sql).empty()) {
                    task.commit();
                }else{
                    throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::ERROR));
                }
            } else {
                throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::ERROR));
            }
        } catch (...){
            task.abort();
            throw;
        }
    }

    void BidRepository::DeleteFinishedBid(const std::string &bid_id) {
        pqxx::work task(db_connection_);
        try{
            std::string participants = "SELECT " + task.quote(BDNames::bid_table_buyer_id) + ", "
                                       + task.quote(BDNames::bid_table_seller_id) + ", "
                                       + " FROM " + task.quote(BDNames::bid_table)
                                       + " WHERE " + task.quote(BDNames::bid_table_id)
                                       + " = " + task.quote(bid_id);
            pqxx::result result = task.exec(participants);
            if (!result[0][BDNames::bid_table_seller_id].is_null()
                && !result[0][BDNames::bid_table_buyer_id].is_null()) {
                std::string sql = "DELETE FROM " + task.quote(BDNames::bid_table)
                                  + " WHERE " + task.quote(BDNames::bid_table_id)
                                  + " = " + task.quote(bid_id);
                task.exec(sql);
                task.commit();
            } else {
                throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::ERROR));
            }
        } catch (...){
            task.abort();
            throw;
        }
    }

    void BidRepository::UpdateBidRate(const std::string &bid_id, const std::string &rate, const std::string &time)  {
        pqxx::work task(db_connection_);
        try{
            std::string sql = "UPDATE " + task.quote(BDNames::bid_table)
                              + " SET " + task.quote(BDNames::bid_table_rate)
                              + " = " + task.quote(rate) + " time = " + task.quote(time)
                              + " WHERE " + task.quote(BDNames::bid_table_id)
                              + " = " + task.quote(bid_id);
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

    void BidRepository::UpdateBidQuantity(const std::string &bid_id, const std::string &quantity, const std::string &time) {
        pqxx::work task(db_connection_);
        try{
            std::string sql = "UPDATE " + task.quote(BDNames::bid_table)
                              + " SET " + task.quote(BDNames::bid_table_quantity)
                              + " = " + task.quote(quantity) + " time = " + task.quote(time)
                              + " WHERE " + task.quote(BDNames::bid_table_id)
                              + " = " + task.quote(bid_id);
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

}