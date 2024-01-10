#ifndef SIMPLESTOCKEXCHANGE_BIDCONTROLLER_HPP
#define SIMPLESTOCKEXCHANGE_BIDCONTROLLER_HPP

#include "../Service/BidServiceInterface.hpp"
#include "../3rdParty/json.hpp"
#include "../Repository/BdNames.hpp"
#include "../Utility/ExtraJSONKeys.hpp"
#include "../Utility/ResponseError.hpp"

namespace s21 {
    class BidController {
    public:

        explicit BidController(BidServiceInterface &service) :service_(service){}

        nlohmann::json CreateBid(const nlohmann::json &request_body);

        nlohmann::json GetBid(const nlohmann::json &request_body);

        nlohmann::json GetUserSellBids(const nlohmann::json &request_body);

        nlohmann::json GetUserBuyBids(const nlohmann::json &request_body);

        nlohmann::json UpdateBidQuantity(const nlohmann::json &request_body);

        nlohmann::json UpdateBidRate(const nlohmann::json &request_body);

        nlohmann::json CancelBid(const nlohmann::json &request_body);

    private:
        BidServiceInterface &service_;
    };
} //s21

#endif //SIMPLESTOCKEXCHANGE_BIDCONTROLLER_HPP
