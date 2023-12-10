#ifndef SIMPLESTOCKEXCHANGE_CLIENTCONTROLLER_HPP
#define SIMPLESTOCKEXCHANGE_CLIENTCONTROLLER_HPP

#include "../Utility/RequestBuilder.hpp"
#include "../3rdParty/json.hpp"
#include "../Repository/BdNames.hpp"
#include "../Service/BidService.hpp"

namespace s21 {
    struct ClientController {

        static std::string Register(const std::string &username, const std::string &password, const std::string &balance){
            nlohmann::json body;
            body[BDNames::user_table_user_name] = username;
            body[BDNames::user_table_password] = password;
            body[BDNames::user_table_balance] = balance;
            return RequestStringBuilder::BuildRequest(RequestStringBuilder::REGISTER, body);
        }
        static std::string Login(const std::string &username, const std::string &password, const std::string &balance){
            nlohmann::json body;
            body[BDNames::user_table_user_name] = username;
            body[BDNames::user_table_password] = password;
            body[BDNames::user_table_balance] = balance;
            return RequestStringBuilder::BuildRequest(RequestStringBuilder::LOGIN, body);
        }
        static std::string CreateBid(const std::string &quantity, const std::string &rate, BidService::BidType type){
            nlohmann::json body;
            body[BDNames::bid_table_quantity] = quantity;
            body[BDNames::bid_table_rate] = rate;
            body[BDNames::bid_table_type] = type;
            return RequestStringBuilder::BuildRequest(RequestStringBuilder::CREATE_BID, body);
        }

        static std::string CancelBid(const std::string &bid_id){
            nlohmann::json body;
            body[BDNames::bid_table_id] = bid_id;
            return RequestStringBuilder::BuildRequest(RequestStringBuilder::CANCEL_BID, body);
        }

        static std::string GetMySellBids(const std::string &user_id){
            nlohmann::json body;
            body[BDNames::user_table_id] = user_id;
            return RequestStringBuilder::BuildRequest(RequestStringBuilder::GET_USER_SELL_BIDS, body);
        }

        static std::string GetMyBuyBids(const std::string &user_id){
            nlohmann::json body;
            body[BDNames::user_table_id] = user_id;
            return RequestStringBuilder::BuildRequest(RequestStringBuilder::GET_USER_BUY_BIDS, body);
        }

        static std::string GetMySellTransactions(const std::string &user_id){
            nlohmann::json body;
            body[BDNames::user_table_id] = user_id;
            return RequestStringBuilder::BuildRequest(RequestStringBuilder::GET_USER_SELL_TRANSACTIONS, body);
        }

        static std::string GetMyBuyTransactions(const std::string &user_id){
            nlohmann::json body;
            body[BDNames::user_table_id] = user_id;
            return RequestStringBuilder::BuildRequest(RequestStringBuilder::GET_USER_BUY_TRANSACTIONS, body);
        }

    };
}

#endif //SIMPLESTOCKEXCHANGE_CLIENTCONTROLLER_HPP
