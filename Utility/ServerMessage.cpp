#include "ServerMessage.hpp"

namespace s21{
    const std::unordered_map<ServerMessage::MessageType, const char*> ServerMessage::server_message{
            {MessageType::WELCOME, "Welcome to Stock Exchange. Please Register or Sign In to continue"},
            {MessageType::NOT_LOGGED_IN, "Operation requires you to be logged in"},
            {MessageType::SERVER_FAILED_TO_START, "Server failed to start"},
            {MessageType::REGISTER_OK, "User registration successful"},
            {MessageType::REGISTER_BAD_NAME, "Username must be at least 4 symbols long"},
            {MessageType::REGISTER_NOT_UNIQUE_NAME, "Username already exists"},
            {MessageType::REGISTER_BAD_PASSWORD,
                "Password must be at least 6 symbols long and contain lower case, upper case and symbols/digits"},
            {MessageType::LOGIN_OK, "Login successful"},
            {MessageType::USER_NOT_FOUND, "Username not found"},
            {MessageType::LOGIN_BAD_PASSWORD, "Wrong password"},
            {MessageType::BID_OK, "Bid created"},
            {MessageType::BID_NOT_FOUND, "Bid not found"},
            {MessageType::BID_BAD_RATE, "Rate must be positive and non-zero"},
            {MessageType::BID_BAD_QUANTITY, "Quantity must be positive and non-zero"},
            {MessageType::TRANSACTION_OK, "Transaction created"},
            {MessageType::TRANSACTION_NOT_FOUND, "Transaction not found"},
            {MessageType::TRANSACTION_INVALID, "Transaction is not valid"},
            {MessageType::REQUEST_BAD_CONTENT_BODY, "Missing body content where it must be present"},
            {MessageType::REQUEST_BAD_CONTENT_TYPE, "Invalid or missing content type"},
            {MessageType::REQUEST_BAD_HOST, "Invalid or missing request host"},
            {MessageType::REQUEST_BAD_METHOD, "Invalid request method"},
            {MessageType::REQUEST_BAD_LENGTH, "Content-Length is required"},
            {MessageType::ERROR, "Internal Error"},
    };
    const std::unordered_map<const char *, ServerMessage::ResponseCode> ServerMessage::response_code{
            {"Welcome to Stock Exchange. Please Register or Sign In to continue", ResponseCode::OK},
            {"Operation requires you to be logged in", ResponseCode::BAD_REQUEST},
            {"Server failed to start", ResponseCode::INTERNAL_SERVER_ERROR},
            {"User registration successful", ResponseCode::OK},
            {"Username must be at least 4 symbols long", ResponseCode::BAD_REQUEST},
            {"Username already exists", ResponseCode::BAD_REQUEST},
            {"Password must be at least 6 symbols long and contain lower case, upper case and symbols/digits",
                ResponseCode::BAD_REQUEST},
            {"Login successful", ResponseCode::OK},
            {"Username not found", ResponseCode::NOT_FOUND},
            {"Wrong password", ResponseCode::BAD_REQUEST},
            {"Bid created", ResponseCode::OK},
            {"Rate must be positive and non-zero", ResponseCode::BAD_REQUEST},
            {"Quantity must be positive and non-zero", ResponseCode::BAD_REQUEST},
            {"Transaction created", ResponseCode::OK},
            {"Transaction not found", ResponseCode::NOT_FOUND},
            {"Transaction is not valid", ResponseCode::BAD_REQUEST},
            {"Missing body content where it must be present", ResponseCode::NOT_FOUND},
            {"Invalid or missing content type", ResponseCode::BAD_REQUEST},
            {"Invalid or missing request host", ResponseCode::BAD_REQUEST},
            {"Invalid request method", ResponseCode::BAD_REQUEST},
            {"Content-Length is required", ResponseCode::BAD_REQUEST},
            {"Internal Error", ResponseCode::BAD_REQUEST}
    };

    const std::unordered_map<ServerMessage::ResponseCode, const char *> ServerMessage::status_message{
            {ServerMessage::OK, "OK"},
            {ServerMessage::BAD_REQUEST, "BAD_REQUEST"},
            {ServerMessage::NOT_FOUND, "NOT FOUND"}
    };
}