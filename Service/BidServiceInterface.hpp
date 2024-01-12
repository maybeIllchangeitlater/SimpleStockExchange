#ifndef SIMPLESTOCKEXCHANGE_BIDSERVICEINTERFACE_HPP
#define SIMPLESTOCKEXCHANGE_BIDSERVICEINTERFACE_HPP

#include <string>

#include "../3rdParty/json.hpp"

namespace s21 {

class BidServiceInterface {
 public:
  enum BidType {
    BUYING,
    SELLING,
  };
  virtual ~BidServiceInterface() = default;
  virtual nlohmann::json CreateBid(const std::string &user_id,
                                   const std::string &rate,
                                   const std::string &quantity,
                                   BidType bid_type) = 0;
  virtual void CancelBid(const std::string &bid_id,
                         const std::string &user_id) = 0;
  virtual nlohmann::json UpdateBidRate(const std::string &bid_id,
                                       const std::string &rate) = 0;
  virtual void UpdateBidQuantity(const std::string &bid_id,
                                 const std::string &quantity) = 0;
  virtual nlohmann::json ReadBid(const std::string &bid_id) = 0;
  virtual nlohmann::json ReadAllUserSellBids(const std::string &user_id) = 0;
  virtual nlohmann::json ReadAllUserBuyBids(const std::string &user_id) = 0;
};

}  // namespace s21

#endif  // SIMPLESTOCKEXCHANGE_BIDSERVICEINTERFACE_HPP
