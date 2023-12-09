#ifndef SIMPLESTOCKEXCHANGE_REQUESTBUILDER_HPP
#define SIMPLESTOCKEXCHANGE_REQUESTBUILDER_HPP

#include <string>
#include <iostream>
#include <unordered_map>
#include "../3rdParty/json.hpp"

namespace s21 {
    struct RequestStringBuilder {
        enum RequestType{
            REGISTER,
            LOGIN,
            GET_USER_BY_ID,
            GET_USER_BY_NAME,
            UPDATE_USER_NAME,
            UPDATE_USER_PASSWORD,
            UPDATE_USER_BALANCE,
            DELETE_USER,
            CREATE_BID,
            GET_BID_BY_ID,
            GET_USER_SELL_BIDS,
            GET_USER_BUY_BIDS,
            UPDATE_BID_RATE,
            UPDATE_BID_QUANTITY,
            CANCEL_BID,
            FINISH_BID,
            MAKE_TRANSACTION,
            GET_TRANSACTION_BY_ID,
            GET_USER_SELL_TRANSACTIONS,
            GET_USER_BUY_TRANSACTIONS,
        };

        static std::string BuildRequest(RequestType type, const nlohmann::json &body){
            auto helper = request_method.at(type);
            std::string request_string(helper.first);
            request_string += " ";
            request_string += helper.second;
            request_string += " HTTP/1.1\nHost: SimpleStockexchangeServer\nContent-Type: application/json\nContent-Length: ";
            std::string dumped_json = body.dump();
            request_string += std::to_string(dumped_json.length());
            request_string += "\r\n\r\n";
            request_string += dumped_json;
            return request_string;
        }

        static const std::unordered_map<RequestType, std::pair<const char *, const char*>> request_method;

    };
    const std::unordered_map<RequestStringBuilder::RequestType, std::pair<const char *, const char*>> RequestStringBuilder::request_method{
            {REGISTER, std::make_pair("POST", "RegisterUser")},
            {LOGIN, std::make_pair("GET", "AuthenticateUser")},
            {GET_USER_BY_ID, std::make_pair("GET", "GetUserById")},
            {GET_USER_BY_NAME, std::make_pair("GET", "GetUserByName")},
            {UPDATE_USER_NAME, std::make_pair("PUT", "UpdateUserName")},
            {UPDATE_USER_PASSWORD, std::make_pair("PUT", "UpdateUserPassword")},
            {UPDATE_USER_BALANCE, std::make_pair("PUT", "UpdateUserBalance")},
            {DELETE_USER, std::make_pair("DELETE", "DeleteUser")},
            {CREATE_BID, std::make_pair("POST", "CreateBid")},
            {GET_BID_BY_ID, std::make_pair("GET", "GetBid")},
            {GET_USER_SELL_BIDS, std::make_pair("GET", "GetUserSellBids")},
            {GET_USER_BUY_BIDS, std::make_pair("GET", "GetUserBuyBids")},
            {UPDATE_BID_RATE, std::make_pair("PUT", "UpdateBidRate")},
            {UPDATE_BID_QUANTITY, std::make_pair("PUT", "UpdateBidQuantity")},
            {CANCEL_BID, std::make_pair("DELETE", "CancelBid")},
            {FINISH_BID, std::make_pair("DELETE", "CloseBid")},
            {MAKE_TRANSACTION, std::make_pair("POST", "MakeTransaction")},
            {GET_TRANSACTION_BY_ID, std::make_pair("GET", "GetTransaction")},
            {GET_USER_BUY_TRANSACTIONS, std::make_pair("GET", "GetUserBuyTransactions")},
            {GET_USER_SELL_TRANSACTIONS, std::make_pair("GET", "GetUserSellTransactions")}
    };
}

#endif //SIMPLESTOCKEXCHANGE_REQUESTBUILDER_HPP
