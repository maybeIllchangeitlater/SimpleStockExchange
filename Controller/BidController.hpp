#ifndef SIMPLESTOCKEXCHANGE_BIDCONTROLLER_HPP
#define SIMPLESTOCKEXCHANGE_BIDCONTROLLER_HPP

#include <vector>
#include "../Service/BidService.hpp"
#include "../3rdParty/json.hpp"

namespace s21 {
    class BidController {
    public:
        explicit BidController(BidService &service) :service_(service){}
        void CreateBid(const nlohmann::json &request_body){
            service_.CreateBid(request_body.at("id"), request_body.at("rate"), request_body.at("quantity"),
                               request_body.at("type"));
        }
        nlohmann::json GetBid(const nlohmann::json &request_body){
            return service_.ReadBid(request_body.at("id"));
        }
        std::vector<nlohmann::json> GetUserSellBids(const nlohmann::json &request_body){
            return service_.ReadAllUserSellBids(request_body.at("id"));
        }
        std::vector<nlohmann::json> GetUserBuyBids(const nlohmann::json &request_body){
            return service_.ReadAllUserBuyBids(request_body.at("id"));
        }
        void UpdateBidQuantity(const nlohmann::json &request_body){
            service_.UpdateBidQuantity(request_body.at("id"), request_body.at("quantity"));
        }
        void UpdateBidRate(const nlohmann::json &request_body){
            service_.UpdateBidRate(request_body.at("id"), request_body.at("rate"));
        }
        void CloseBid(const nlohmann::json &request_body){
            service_.CloseBid(request_body.at("id"));
        }
        void CancelBid(const nlohmann::json &request_body){
            service_.CancelBid(request_body.at("id"));
        }

    private:
        BidService &service_;
    };
} //s21

#endif //SIMPLESTOCKEXCHANGE_BIDCONTROLLER_HPP
