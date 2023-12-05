#include "BidRepository.hpp"

namespace s21{

    void BidRepository::CreateSellBid(const std::string &bid_id, const std::string &seller_id, const std::string &rate,
                                      const std::string &quantity, const std::string &timestamp) {
        pqxx::work task(db_connection_);
        try{

            std::string sql = "INSERT INTO bid_info (id, seller_id, rate, quantity, time) VALUES ("
                              + task.quote(bid_id) + ", "
                              + task.quote(seller_id) + ", " + task.quote(rate) + ", "
                              + task.quote(quantity) + ", " + task.quote(timestamp) + ")";
            if(!task.exec(sql).empty()) {
                task.commit();
            }else{
                throw std::runtime_error("Error Creating Bid");
            }
        }catch(...){
            task.abort();
            throw std::runtime_error("Error Creating Bid");
        }
    }

    void BidRepository::CreateBuyBid(const std::string &bid_id, const std::string &buyer_id, const std::string &rate,
                                     const std::string &quantity, const std::string &timestamp) {
        pqxx::work task(db_connection_);
        try{

            std::string sql = "INSERT INTO bid_info (id, buyer_id, rate, quantity, time) VALUES ("
                              + task.quote(bid_id) + ", "
                              + task.quote(buyer_id) + ", " + task.quote(rate) + ", "
                              + task.quote(quantity) +  ", " + task.quote(timestamp) + ")";
            if(!task.exec(sql).empty()) {
                task.commit();
            }else{
                throw std::runtime_error("Error Creating Bid");
            }
        }catch(...){
            task.abort();
            throw std::runtime_error("Error Creating Bid");
        }
    }

    pqxx::result BidRepository::ReadBid(const std::string &bid_id) {
        pqxx::work task(db_connection_);
        try{

            std::string sql = "SELECT b.id AS bid id, seller.name AS seller username,"
                              "buyer.name AS buyer username, b.rate, b.quantity, b.time FROM bid_info b "
                              "JOIN user_info seller ON b.seller_id = seller.id "
                              "JOIN user_info buyer ON b.buyer_id = buyer.id "
                              "WHERE id = " + task.quote(bid_id);
            pqxx::result res = task.exec(sql);
            if(!res.empty()) {
                return res;
            }else{
                throw std::runtime_error ("Error Reading Bid");
            }
        }catch(...){
            task.abort();
            throw std::runtime_error ("Error Reading Bid");
        }
    }

    pqxx::result BidRepository::ReadAllUserSellBids(const std::string &seller_id) {
        pqxx::work task(db_connection_);
        try{

            std::string sql = "SELECT b.id AS bid_id, seller.name AS seller username,"
                              "buyer.name AS buyer username, b.rate, b.quantity, b.time FROM bid_info b "
                              "JOIN user_info seller ON b.seller_id = seller.id "
                              "JOIN user_info buyer ON b.buyer_id = buyer.id "
                              "WHERE seller.id = " + task.quote(seller_id);
            pqxx::result res = task.exec(sql);
            if(!res.empty()) {
                return res;
            }else{
                throw std::runtime_error ("Error Reading User Sell Bids");
            }
        }catch(...){
            task.abort();
            throw std::runtime_error ("Error Reading User Sell Bids");
        }
    }

    pqxx::result BidRepository::ReadAllUserBuyBids(const std::string &buyer_id) {
        pqxx::work task(db_connection_);
        try{

            std::string sql = "SELECT b.id AS bid_id, seller.name AS seller username,"
                              "buyer.name AS buyer username, b.rate, b.quantity, b.time FROM bid_info b "
                              "JOIN user_info seller ON b.seller_id = seller.id "
                              "JOIN user_info buyer ON b.buyer_id = buyer.id "
                              "WHERE buyer.id = " + task.quote(buyer_id);
            pqxx::result res = task.exec(sql);
            if(!res.empty()) {
                return res;
            }else{
                throw std::runtime_error ("Error Reading User Buy Bids");
            }
        }catch(...){
            task.abort();
            throw std::runtime_error ("Error Reading User Buy Bids");
        }
    }

    void BidRepository::DeleteUnfinishedBid(const std::string &bid_id) {
        pqxx::work task(db_connection_);
        try{
            std::string buyer = "SELECT buyer_id, seller_id, FROM bid_info WHERE id = " + task.quote(bid_id);
            pqxx::result result = task.exec(buyer);
            if (result[0]["seller_id"].is_null() || result[0]["buyer_id"].is_null()) {
                std::string sql = "DELETE FROM bid_info WHERE id = " + task.quote(bid_id);
                task.exec(sql);
                task.commit();
            } else {
            throw std::logic_error("Cannot Cancel Finished Transaction");
        }
    } catch (...){
            task.abort();
            throw std::runtime_error("Error Cancelling Transaction");
        }
    }

    void BidRepository::DeleteFinishedBid(const std::string &bid_id) {
        pqxx::work task(db_connection_);
        try{
            std::string buyer = "SELECT buyer_id, seller_id, FROM bid_info WHERE id = " + task.quote(bid_id);
            pqxx::result result = task.exec(buyer);
            if (!result[0]["seller_id"].is_null() && !result[0]["buyer_id"].is_null()) {
                std::string sql = "DELETE FROM bid_info WHERE id = " + task.quote(bid_id);
                task.exec(sql);
                task.commit();
            } else {
                throw std::logic_error("Cannot Finish Unfinished Transaction");
            }
        } catch (...){
            task.abort();
            throw std::runtime_error("Error Finishing Transaction");
        }
    }

    void BidRepository::UpdateBidRate(const std::string &bid_id, const std::string &rate, const std::string &time)  {
        pqxx::work task(db_connection_);
        try{
            std::string sql = "UPDATE bid_info SET rate = " + task.quote(rate)
                    + " time = " + task.quote(time) + " WHERE id = " + task.quote(bid_id);
            if(!task.exec(sql).empty()) {
                task.commit();
            }else{
                throw std::runtime_error("Error Updating Bid Rate");
            }
        }catch(...){
            task.abort();
            throw std::runtime_error("Error Updating Bid Rate");
        }
    }

    void BidRepository::UpdateBidQuantity(const std::string &bid_id, const std::string &quantity, const std::string &time) {
        pqxx::work task(db_connection_);
        try{
            std::string sql = "UPDATE bid_info SET quantity = " + task.quote(quantity)
                    + " time = " + task.quote(time) + " WHERE id = " + task.quote(bid_id);
            if(!task.exec(sql).empty()) {
                task.commit();
            }else{
                throw std::runtime_error("Error Updating Bid Quantity");
            }
        }catch(...){
            task.abort();
            throw std::runtime_error("Error Updating Bid Quantity");
        }
    }

}