#ifndef SIMPLESTOCKEXCHANGE_TRANSACTIONCONTROLLER_HPP
#define SIMPLESTOCKEXCHANGE_TRANSACTIONCONTROLLER_HPP

#include <vector>
#include "../Service/TransactionService.hpp"
#include "../3rdParty/json.hpp"
#include "../Repository/BdNames.hpp"

namespace s21 {
    class TransactionController {
    public:
        explicit TransactionController(TransactionService &service) : service_(service){}

        nlohmann::json MakeTransaction(const nlohmann::json &request_body);

        nlohmann::json GetTransaction(const nlohmann::json &request_body);

        nlohmann::json GetUserSellTransactions(const nlohmann::json &request_body);

        nlohmann::json GetUserBuyTransactions(const nlohmann::json &request_body);

    private:
        TransactionService &service_;
    };
}

#endif //SIMPLESTOCKEXCHANGE_TRANSACTIONCONTROLLER_HPP
//pack / unpuck to http to request/responce class

//cascade delete in

///lock to service