#ifndef SIMPLESTOCKEXCHANGE_HTTP_HPP
#define SIMPLESTOCKEXCHANGE_HTTP_HPP

#include <string>
#include <unordered_map>
#include <boost/asio.hpp>
#include "../3rdParty/json.hpp"
#include "../Utility/ServerMessage.hpp"
#include <iostream>

namespace s21 {
    struct Request {

        size_t content_length;
        std::string method;
        std::string path;
        nlohmann::json body;

        static Request Parse(const std::string &raw_request_header){
            Request request;
            std::istringstream request_stream(raw_request_header);
            std::string line;
            std::string checker;
            if (std::getline(request_stream, line)) {
                std::istringstream line_stream(line);
                line_stream >> request.method >> request.path >> line; //skipping protocol ver
                if(request.method != "GET" && request.method != "POST" && request.method != "PUT"
                    && request.method != "DELETE"){
                    throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::REQUEST_BAD_METHOD));
                }
            }else{
                throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::ERROR));
            }
            if (std::getline(request_stream, line)) {
                std::istringstream line_stream(line);
                line_stream >> line >> checker;
                if(line != "Host:" || (checker != "SimpleStockexchangeServer" && checker != "SimpleStockexchangeClient")){
                    throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::REQUEST_BAD_HOST));
                }
            }else{
                throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::ERROR));
            }
            if (std::getline(request_stream, line)) {
                std::istringstream line_stream(line);
                line_stream >> line >> checker;
                if(line != "Content-Type:" || checker != "application/json")
                    throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::REQUEST_BAD_CONTENT_TYPE));
            }else{
                throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::ERROR));
            }
            if (std::getline(request_stream, line)) {
                std::istringstream line_stream(line);
                line_stream >> line >> checker;
                if(line != "Content-Length:" || checker.empty()){
                    throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::REQUEST_BAD_LENGTH));
                }
            }else{
                throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::ERROR));
            }
            if(!std::getline(request_stream, line))
                throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::ERROR));
            if(std::getline(request_stream, line)) {
                    request.body = nlohmann::json::parse(line);
            }
            return request;
        }

    };

    struct Response{
        nlohmann::json body;
        ServerMessage::ResponseCode status;

        std::string ToString() const {
            return "HTTP/1.1 " + std::to_string(status) + " " + ServerMessage::status_message.at(status) + "\nContent-Length: "
            + std::to_string(body.dump().size()) + "\r\n\r\n" + body.dump();
        }
    };
}

#endif //SIMPLESTOCKEXCHANGE_HTTP_HPP
