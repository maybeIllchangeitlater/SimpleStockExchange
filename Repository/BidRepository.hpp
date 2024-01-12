#ifndef SIMPLESTOCKEXCHANGE_BIDREPOSITORY_HPP
#define SIMPLESTOCKEXCHANGE_BIDREPOSITORY_HPP

#include "../3rdParty/libpqxx/include/pqxx/pqxx"
#include "../Utility/ServerMessage.hpp"
#include "BdNames.hpp"
#include "DatabaseInterface.hpp"

namespace s21 {
class BidRepository {
 public:
  explicit BidRepository(DatabaseInterface &db) : db_(db) {}

  void CreateSellBid(const std::string &seller_id, const std::string &rate,
                     const std::string &quantity, const std::string &timestamp);
  void CreateBuyBid(const std::string &buyer_id, const std::string &rate,
                    const std::string &quantity, const std::string &timestamp);

  pqxx::result ReadBid(const std::string &bid_id);
  /// read bid in select * format. user ids are returned instead of names
  pqxx::result ReadBidRaw(const std::string &bid_id);
  pqxx::result ReadAllUserSellBids(const std::string &seller_id);
  pqxx::result ReadAllUserBuyBids(const std::string &buyer_id);
  /// match sell bids against fresh buy bid
  pqxx::result MatchSellBids(const std::string &user_id,
                             const std::string &rate);
  /// match buy bids against fresh sell bid
  pqxx::result MatchBuyBids(const std::string &user_id,
                            const std::string &rate);
  void UpdateBidRate(const std::string &bid_id, const std::string &rate,
                     const std::string &time);
  void UpdateBidQuantity(const std::string &bid_id, const std::string &quantity,
                         const std::string &time);
  void DeleteBid(const std::string &bid_id);

 private:
  DatabaseInterface &db_;
};
}  // namespace s21

#endif  // SIMPLESTOCKEXCHANGE_BIDREPOSITORY_HPP
