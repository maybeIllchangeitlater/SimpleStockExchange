#include "UserController.hpp"

namespace s21 {
nlohmann::json UserController::AuthenticateUser(
    const nlohmann::json &request_body) {
  nlohmann::json response;
  try {
    auto user =
        service_.GetUserByName(request_body.at(BDNames::user_table_user_name));
    if (user.at(BDNames::user_table_password) ==
        Encoder::Encode(request_body.at(BDNames::user_table_password))) {
      response[ExtraJSONKeys::status] = ServerMessage::ResponseCode::OK;
      response[BDNames::user_table_id] = user.at(BDNames::user_table_id);
      return response;
    } else {
      throw std::runtime_error(
          ServerMessage::server_message.at(ServerMessage::LOGIN_BAD_PASSWORD));
    }
  } catch (const std::exception &e) {
    ResponseError::Error(response, e.what());
    return response;
  }
}

nlohmann::json UserController::RegisterUser(
    const nlohmann::json &request_body) {
  nlohmann::json response;
  try {
    auto check_if_exists =
        service_.GetUserByName(request_body.at(BDNames::user_table_user_name));
    if (!check_if_exists.empty()) {
      ResponseError::Error(response,
                           ServerMessage::server_message.at(
                               ServerMessage::REGISTER_NOT_UNIQUE_NAME));
      return response;
    }
  } catch (const std::exception &e) {
    try {
      service_.CreateUser(request_body.at(BDNames::user_table_user_name),
                          request_body.at(BDNames::user_table_password),
                          request_body.at(BDNames::balance_table_usd),
                          request_body.at(BDNames::balance_table_rub));
      auto user = service_.GetUserByName(
          request_body.at(BDNames::user_table_user_name));
      response[ExtraJSONKeys::status] = ServerMessage::ResponseCode::OK;
      response[BDNames::user_table_id] = user.at(BDNames::user_table_id);
      return response;
    } catch (const std::exception &e) {
      ResponseError::Error(response, e.what());
      return response;
    }
  }
  return response;  // for compiler warnings
}

nlohmann::json UserController::GetUserById(const nlohmann::json &request_body) {
  nlohmann::json response;
  try {
    response = service_.GetUserById(request_body.at(BDNames::user_table_id));
    response.erase(BDNames::user_table_password);
    response[ExtraJSONKeys::status] = ServerMessage::ResponseCode::OK;
    return response;
  } catch (const std::exception &e) {
    ResponseError::Error(response, e.what());
    return response;
  }
}

nlohmann::json UserController::GetUserBalance(
    const nlohmann::json &request_body) {
  nlohmann::json response;
  try {
    auto tmp = GetUserById(request_body);
    response[BDNames::balance_table_usd] = tmp[BDNames::balance_table_usd];
    response[BDNames::balance_table_rub] = tmp[BDNames::balance_table_rub];
    response[ExtraJSONKeys::status] = ServerMessage::ResponseCode::OK;
    return response;
  } catch (const std::exception &e) {
    ResponseError::Error(response, e.what());
    return response;
  }
}

nlohmann::json UserController::GetUserByName(
    const nlohmann::json &request_body) {
  nlohmann::json response;
  try {
    response =
        service_.GetUserByName(request_body.at(BDNames::user_table_user_name));
    response.erase(BDNames::user_table_password);
    response[ExtraJSONKeys::status] = ServerMessage::ResponseCode::OK;
    return response;
  } catch (const std::exception &e) {
    ResponseError::Error(response, e.what());
    return response;
  }
}

nlohmann::json UserController::UpdateUserName(
    const nlohmann::json &request_body) {
  nlohmann::json response;
  try {
    service_.UpdateUserName(request_body.at(BDNames::user_table_id),
                            request_body.at(BDNames::user_table_user_name));
    response[ExtraJSONKeys::status] = ServerMessage::ResponseCode::OK;
    return response;
  } catch (const std::exception &e) {
    ResponseError::Error(response, e.what());
    return response;
  }
}

nlohmann::json UserController::UpdateUserPassword(
    const nlohmann::json &request_body) {
  nlohmann::json response;
  try {
    service_.UpdateUserPassword(request_body.at(BDNames::user_table_id),
                                request_body.at(BDNames::user_table_password),
                                request_body.at(ExtraJSONKeys::old_password));
    response[ExtraJSONKeys::status] = ServerMessage::ResponseCode::OK;
    return response;
  } catch (const std::exception &e) {
    ResponseError::Error(response, e.what());
    return response;
  }
}

nlohmann::json UserController::UpdateUserBalance(
    const nlohmann::json &request_body) {
  nlohmann::json response;
  try {
    service_.UpdateUserBalance(request_body.at(BDNames::user_table_id),
                               request_body.at(BDNames::balance_table_usd),
                               request_body.at(BDNames::balance_table_rub));
    response[ExtraJSONKeys::status] = ServerMessage::ResponseCode::OK;
    return response;
  } catch (const std::exception &e) {
    ResponseError::Error(response, e.what());
    return response;
  }
}

nlohmann::json UserController::DeleteUser(const nlohmann::json &request_body) {
  nlohmann::json response;
  try {
    service_.DeleteUser(request_body.at(BDNames::user_table_id),
                        request_body.at(BDNames::user_table_password));
    response[ExtraJSONKeys::status] = ServerMessage::ResponseCode::OK;
    return response;
  } catch (const std::exception &e) {
    ResponseError::Error(response, e.what());
    return response;
  }
}
}  // namespace s21