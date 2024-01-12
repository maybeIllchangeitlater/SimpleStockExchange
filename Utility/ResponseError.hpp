#ifndef SIMPLESTOCKEXCHANGE_UTILITY_RESPONSEERROR_HPP
#define SIMPLESTOCKEXCHANGE_UTILITY_RESPONSEERROR_HPP

#include "../3rdParty/json.hpp"
#include "ExtraJSONKeys.hpp"
#include "ServerMessage.hpp"

namespace s21 {
struct ResponseError {
  /// Adds response error to json based on caught exception
  static void Error(nlohmann::json &response, const char *exception) {
    response[ExtraJSONKeys::status] =
        ServerMessage::response_code.find(exception) !=
                ServerMessage::response_code.end()
            ? ServerMessage::response_code.at(exception)
            : ServerMessage::ResponseCode::BAD_REQUEST;
    response[ExtraJSONKeys::message] = exception;
  }
};
}  // namespace s21

#endif  // SIMPLESTOCKEXCHANGE_UTILITY_RESPONSEERROR_HPP
