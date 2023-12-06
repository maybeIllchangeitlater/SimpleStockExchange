#ifndef SIMPLESTOCKEXCHANGE_TRANSACTIONCONTROLLER_HPP
#define SIMPLESTOCKEXCHANGE_TRANSACTIONCONTROLLER_HPP

#include <vector>
#include "../Service/TransactionService.hpp"
#include "../3rdParty/json.hpp"

namespace s21 {
    class TransactionController {
    public:
        explicit TransactionController(TransactionService &service) : service_(service){}
        void MakeTransaction(const nlohmann::json &request_body){
            service_.MakeTransaction(request_body.at("seller_id"), request_body.at("buyer_id"),
                                     request_body.at("rate"), request_body.at("quantity"));
        }
        nlohmann::json GetTransaction(const nlohmann::json &request_body){
            return service_.ReadTransaction(request_body.at("id"));
        }
        std::vector<nlohmann::json> GetUserSellTransactions(const nlohmann::json &request_body){
            return service_.ReadAllUserSellTransactions(request_body.at("id"));
        }
        std::vector<nlohmann::json> GetUserBuyTransactions(const nlohmann::json &request_body){
            return service_.ReadAllUserBuyTransactions(request_body.at("id"));
        }

    private:
        TransactionService &service_;
    };
}

#endif //SIMPLESTOCKEXCHANGE_TRANSACTIONCONTROLLER_HPP
//pack / unpuck to http to request/responce class

//cascade delete in

///lock to service