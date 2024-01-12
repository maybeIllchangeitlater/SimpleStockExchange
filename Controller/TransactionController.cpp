#include "TransactionController.hpp"

namespace s21 {

nlohmann::json TransactionController::GetUserSellTransactions(
    const nlohmann::json &request_body) {
  nlohmann::json response;
  try {
    response = service_.ReadAllUserSellTransactions(
        request_body.at(BDNames::transaction_table_id));
    nlohmann::json status;
    status[ExtraJSONKeys::status] = ServerMessage::OK;
    response.emplace_back(status);
    return response;
  } catch (const std::exception &e) {
    ResponseError::Error(response, e.what());
    return response;
  }
}

nlohmann::json TransactionController::GetUserBuyTransactions(
    const nlohmann::json &request_body) {
  nlohmann::json response;
  try {
    response = service_.ReadAllUserBuyTransactions(
        request_body.at(BDNames::transaction_table_id));
    nlohmann::json status;
    status[ExtraJSONKeys::status] = ServerMessage::OK;
    response.emplace_back(status);
    return response;
  } catch (const std::exception &e) {
    ResponseError::Error(response, e.what());
    return response;
  }
}

nlohmann::json TransactionController::GetQuotations(
    const nlohmann::json &request_body) {
  nlohmann::json response;
  try {
    response =
        service_.GetQuotations(request_body.at(ExtraJSONKeys::time_period));
    response[ExtraJSONKeys::status] = ServerMessage::OK;
    return response;
  } catch (const std::exception &e) {
    ResponseError::Error(response, e.what());
    return response;
  }
}
}  // namespace s21