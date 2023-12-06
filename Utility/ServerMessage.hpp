#ifndef SIMPLESTOCKEXCHANGE_SERVERMESSAGE_HPP
#define SIMPLESTOCKEXCHANGE_SERVERMESSAGE_HPP

#include <string>
#include <unordered_map>

namespace s21 {
    struct ServerMessage {
        enum MessageType {
            WELCOME,
            REGISTER_OK,
            REGISTER_BAD_NAME,
            REGISTER_NOT_UNIQUE_NAME,
            REGISTER_BAD_PASSWORD,
            LOGIN_OK,
            USER_NOT_FOUND,
            LOGIN_BAD_PASSWORD,
            BID_OK,
            BID_NOT_FOUND,
            BID_BAD_RATE,
            BID_BAD_QUANTITY,
            TRANSACTION_OK,
            TRANSACTION_NOT_FOUND,
            TRANSACTION_INVALID,
            REQUEST_BAD_HOST,
            REQUEST_BAD_CONTENT_TYPE,
            REQUEST_BAD_CONTENT_BODY,
            REQUEST_BAD_METHOD,
            ERROR,
        };
        enum ResponseCode {
            OK = 200,
            BAD_REQUEST = 400,
            NOT_FOUND = 404,
        };
        static const std::unordered_map<ServerMessage::MessageType, const char*> server_message;
        static const std::unordered_map<const char *, ServerMessage::ResponseCode> response_code;
    };
} //s21

#endif //SIMPLESTOCKEXCHANGE_SERVERMESSAGE_HPP
