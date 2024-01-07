#ifndef SIMPLESTOCKEXCHANGE_CLIENTCONTROLLER_HPP
#define SIMPLESTOCKEXCHANGE_CLIENTCONTROLLER_HPP

#include "../Utility/RequestBuilder.hpp"
#include "../3rdParty/json.hpp"
#include "../Repository/BdNames.hpp"
#include "../Service/BidService.hpp"
#include "../Utility/ExtraJSONKeys.hpp"

namespace s21 {
    struct ClientController {

        static std::string Register(const std::string &username, const std::string &password,
                                    const std::string &balance_usd, const std::string &balance_rub){
            nlohmann::json body;
            body[BDNames::user_table_user_name] = username;
            body[BDNames::user_table_password] = password;
            body[BDNames::balance_table_usd] = balance_usd;
            body[BDNames::balance_table_rub] = balance_rub;
            return RequestStringBuilder::BuildRequest(RequestStringBuilder::REGISTER, body);
        }
        static std::string Login(const std::string &username, const std::string &password){
            nlohmann::json body;
            body[BDNames::user_table_user_name] = username;
            body[BDNames::user_table_password] = password;
            return RequestStringBuilder::BuildRequest(RequestStringBuilder::LOGIN, body);
        }
        static std::string CreateBid(const std::string &user_id, const std::string &quantity, const std::string &rate, BidService::BidType type){
            nlohmann::json body;
            body[type == BidService::BUYING ? BDNames::bid_table_buyer_id : BDNames::bid_table_seller_id] = user_id;
            body[BDNames::bid_table_quantity] = quantity;
            body[BDNames::bid_table_rate] = rate;
            body[BDNames::bid_table_type] = type;
            return RequestStringBuilder::BuildRequest(RequestStringBuilder::CREATE_BID, body);
        }

        static std::string CancelBid(const std::string &bid_id, const std::string &user_id){
            nlohmann::json body;
            body[BDNames::bid_table_id] = bid_id;
            body[BDNames::trader_id] = user_id;
            return RequestStringBuilder::BuildRequest(RequestStringBuilder::CANCEL_BID, body);
        }

        static std::string UpdateBidRate(const std::string &bid_id, const std::string &new_rate){
            nlohmann::json body;
            body[BDNames::bid_table_id] = bid_id;
            body[BDNames::bid_table_rate] = new_rate;
            return RequestStringBuilder::BuildRequest(RequestStringBuilder::UPDATE_BID_RATE, body);
        }

        static std::string UpdateBidQuantity(const std::string &bid_id, const std::string &new_quantity){
            nlohmann::json body;
            body[BDNames::bid_table_id] = bid_id;
            body[BDNames::bid_table_quantity] = new_quantity;
            return RequestStringBuilder::BuildRequest(RequestStringBuilder::UPDATE_BID_QUANTITY, body);
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

        static std::string GetMyId(){
            nlohmann::json body;
            return RequestStringBuilder::BuildRequest(RequestStringBuilder::GET_MY_ID, body);
        }

        static std::string ChangeUserName(const std::string &user_id, const std::string &new_username){
            nlohmann::json body;
            body[BDNames::user_table_user_name] = new_username;
            body[BDNames::user_table_id] = user_id;
            return RequestStringBuilder::BuildRequest(RequestStringBuilder::UPDATE_USER_NAME, body);
        }

        static std::string ChangePassword(const std::string &user_id,
                                          const std::string &new_password, const std::string &old_password){
            nlohmann::json body;
            body[BDNames::user_table_password] = new_password;
            body[ExtraJSONKeys::old_password] = old_password;
            body[BDNames::user_table_id] = user_id;
            return RequestStringBuilder::BuildRequest(RequestStringBuilder::UPDATE_USER_PASSWORD, body);
        }

        static std::string GetMyBalance(const std::string &user_id){
            nlohmann::json body;
            body[BDNames::user_table_id] = user_id;
            return RequestStringBuilder::BuildRequest(RequestStringBuilder::GET_MY_BALANCE, body);
        }

        static std::string GetQuotations(std::string &&time_period){
            nlohmann::json body;
            body[ExtraJSONKeys::time_period] = std::move(time_period);
            return RequestStringBuilder::BuildRequest(RequestStringBuilder::GET_QUOTATIONS, body);
        }

        static std::string DeleteMe(const std::string &user_id, const std::string &user_password){
            nlohmann::json body;
            body[BDNames::user_table_id] = user_id;
            body[BDNames::user_table_password] = user_password;
            return RequestStringBuilder::BuildRequest(RequestStringBuilder::DELETE_USER, body);
        }

        static std::string Logout(){
            nlohmann::json body;
            body["aboba"] = "biba";
            return RequestStringBuilder::BuildRequest(RequestStringBuilder::LOGOUT, body);
        }
    };
}

#endif //SIMPLESTOCKEXCHANGE_CLIENTCONTROLLER_HPP
