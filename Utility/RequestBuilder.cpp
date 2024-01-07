#include "RequestBuilder.hpp"

namespace s21{
    const std::unordered_map<RequestStringBuilder::RequestType, std::pair<const char *, const char*>> RequestStringBuilder::request_method{
            {REGISTER, std::make_pair("POST", "URegisterUser")},
            {LOGIN, std::make_pair("GET", "UAuthenticateUser")},
            {LOGOUT, std::make_pair("DELETE", "DDCMe")},
            {GET_USER_BY_ID, std::make_pair("GET", "UGetUserById")},
            {GET_USER_BY_NAME, std::make_pair("GET", "UGetUserByName")},
            {GET_MY_ID, std::make_pair("GET", "GReturnId")},
            {GET_MY_BALANCE, std::make_pair("GET", "UGetUserBalance")},
            {UPDATE_USER_NAME, std::make_pair("PUT", "UUpdateUserName")},
            {UPDATE_USER_PASSWORD, std::make_pair("PUT", "UUpdateUserPassword")},
            {UPDATE_USER_BALANCE, std::make_pair("PUT", "UUpdateUserBalance")},
            {DELETE_USER, std::make_pair("DELETE", "UDeleteUser")},
            {CREATE_BID, std::make_pair("POST", "BCreateBid")},
            {GET_BID_BY_ID, std::make_pair("GET", "BGetBid")},
            {GET_USER_SELL_BIDS, std::make_pair("GET", "BGetUserSellBids")},
            {GET_USER_BUY_BIDS, std::make_pair("GET", "BGetUserBuyBids")},
            {UPDATE_BID_RATE, std::make_pair("PUT", "BUpdateBidRate")},
            {UPDATE_BID_QUANTITY, std::make_pair("PUT", "BUpdateBidQuantity")},
            {CANCEL_BID, std::make_pair("DELETE", "BCancelBid")},
            {FINISH_BID, std::make_pair("DELETE", "BCloseBid")},
            {MAKE_TRANSACTION, std::make_pair("POST", "TMakeTransaction")},
            {GET_TRANSACTION_BY_ID, std::make_pair("GET", "TGetTransaction")},
            {GET_USER_BUY_TRANSACTIONS, std::make_pair("GET", "TGetUserBuyTransactions")},
            {GET_USER_SELL_TRANSACTIONS, std::make_pair("GET", "TGetUserSellTransactions")},
            {GET_QUOTATIONS, std::make_pair("GET", "TGetQuotations")}
    };

    std::string RequestStringBuilder::BuildRequest(s21::RequestStringBuilder::RequestType type,
                                                   const nlohmann::json &body) {
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
}
