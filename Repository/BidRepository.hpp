#ifndef SIMPLESTOCKEXCHANGE_BIDREPOSITORY_HPP
#define SIMPLESTOCKEXCHANGE_BIDREPOSITORY_HPP

#include "../3rdParty/libpqxx/include/pqxx/pqxx"
#include "BdNames.hpp"
#include "../Utility/UUIDGenerator.hpp"
#include "../Utility/ServerMessage.hpp"

namespace s21 {
    class BidRepository {
    public:
        explicit BidRepository(pqxx::connection &db_conn) : db_connection_(db_conn) {}

        void CreateSellBid(const std::string &bid_id, const std::string &seller_id,
                                   const std::string& rate, const std::string &quantity,
                                   const std::string& timestamp);
        void CreateBuyBid(const std::string &bid_id, const std::string &buyer_id,
                                  const std::string& rate, const std::string &quantity,
                                    const std::string& timestamp);

        pqxx::result ReadBid(const std::string &bid_id);
        pqxx::result ReadAllUserSellBids(const std::string &seller_id);
        pqxx::result ReadAllUserBuyBids(const std::string &buyer_id);
        void UpdateBidRate(const std::string &bid_id, const std::string &rate, const std::string &time);
        void UpdateBidQuantity(const std::string &bid_id, const std::string &quantity, const std::string &time);
        void DeleteUnfinishedBid(const std::string &bid_id);
        void DeleteFinishedBid(const std::string &bid_id);
    private:
        pqxx::connection &db_connection_;
    };
} //s21

#endif //SIMPLESTOCKEXCHANGE_BIDREPOSITORY_HPP
