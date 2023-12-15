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
            LOGOUT,
            GET_USER_BY_ID,
            GET_USER_BY_NAME,
            GET_MY_ID,
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

        static std::string BuildRequest(RequestType type, const nlohmann::json &body);

        static const std::unordered_map<RequestType, std::pair<const char *, const char*>> request_method;

    };
}

#endif //SIMPLESTOCKEXCHANGE_REQUESTBUILDER_HPP
