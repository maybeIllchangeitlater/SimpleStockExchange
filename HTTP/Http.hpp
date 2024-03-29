#ifndef SIMPLESTOCKEXCHANGE_HTTP_HPP
#define SIMPLESTOCKEXCHANGE_HTTP_HPP

#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <unordered_map>

#include "../3rdParty/json.hpp"
#include "../Utility/ServerMessage.hpp"

namespace s21 {
struct Request {
  std::string method;
  std::string path;
  nlohmann::json body;
  /// parse raw request and store it into a structured object
  static Request Parse(const std::string &raw_request_header) {
    Request request;
    std::istringstream request_stream(raw_request_header);
    std::string line;
    std::string checker;
    if (std::getline(request_stream, line)) {
      std::istringstream line_stream(line);
      line_stream >> request.method >> request.path >>
          line;  // skipping protocol ver
      if (request.method != "GET" && request.method != "POST" &&
          request.method != "PUT" && request.method != "DELETE") {
        throw std::runtime_error(ServerMessage::server_message.at(
            ServerMessage::REQUEST_BAD_METHOD));
      }
    } else {
      throw std::runtime_error(
          ServerMessage::server_message.at(ServerMessage::ERROR));
    }
    if (std::getline(request_stream, line)) {
      std::istringstream line_stream(line);
      line_stream >> line >> checker;
      if (line != "Host:" || (checker != "SimpleStockexchangeServer" &&
                              checker != "SimpleStockexchangeClient")) {
        throw std::runtime_error(
            ServerMessage::server_message.at(ServerMessage::REQUEST_BAD_HOST));
      }
    } else {
      throw std::runtime_error(
          ServerMessage::server_message.at(ServerMessage::ERROR));
    }
    if (std::getline(request_stream, line)) {
      std::istringstream line_stream(line);
      line_stream >> line >> checker;
      if (line != "Content-Type:" || checker != "application/json")
        throw std::runtime_error(ServerMessage::server_message.at(
            ServerMessage::REQUEST_BAD_CONTENT_TYPE));
    } else {
      throw std::runtime_error(
          ServerMessage::server_message.at(ServerMessage::ERROR));
    }
    if (std::getline(request_stream, line)) {
      std::istringstream line_stream(line);
      line_stream >> line >> checker;
      if (line != "Content-Length:" || checker.empty()) {
        throw std::runtime_error(ServerMessage::server_message.at(
            ServerMessage::REQUEST_BAD_LENGTH));
      }
    } else {
      throw std::runtime_error(
          ServerMessage::server_message.at(ServerMessage::ERROR));
    }
    if (!std::getline(request_stream, line))
      throw std::runtime_error(
          ServerMessage::server_message.at(ServerMessage::ERROR));
    if (std::getline(request_stream, line)) {
      request.body = nlohmann::json::parse(line);
    }
    return request;
  }
};

struct Response {
  nlohmann::json body;
  ServerMessage::ResponseCode status;
  /// create http response based on json object
  std::string ToString() const {
    return "HTTP/1.1 " + std::to_string(status) + " " +
           ServerMessage::status_message.at(status) +
           "\nContent-Length: " + std::to_string(body.dump().size()) +
           "\r\n\r\n" + body.dump();
    //+ "\r\n\r\n\r";
  }
};
}  // namespace s21

#endif  // SIMPLESTOCKEXCHANGE_HTTP_HPP
