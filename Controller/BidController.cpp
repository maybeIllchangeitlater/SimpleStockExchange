#include "BidController.hpp"

#include <iostream>

namespace s21 {
nlohmann::json BidController::CreateBid(const nlohmann::json &request_body) {
  nlohmann::json response;
  try {
    response =
        service_.CreateBid(request_body.at(BDNames::bid_table_type) == 1
                               ? request_body.at(BDNames::bid_table_seller_id)
                               : request_body.at(BDNames::bid_table_buyer_id),
                           request_body.at(BDNames::bid_table_rate),
                           request_body.at(BDNames::bid_table_quantity),
                           request_body.at(BDNames::bid_table_type));
    nlohmann::json status;
    status[ExtraJSONKeys::status] = ServerMessage::OK;
    response.emplace_back(status);
    return response;
  } catch (const std::exception &e) {
    ResponseError::Error(response, e.what());
    return response;
  }
}

nlohmann::json BidController::GetBid(const nlohmann::json &request_body) {
  nlohmann::json response;
  try {
    response = service_.ReadBid(request_body.at(BDNames::bid_table_id));
    response[ExtraJSONKeys::status] = ServerMessage::ResponseCode::OK;
    return response;
  } catch (const std::exception &e) {
    ResponseError::Error(response, e.what());
    return response;
  }
}

nlohmann::json BidController::GetUserBuyBids(
    const nlohmann::json &request_body) {
  nlohmann::json response;
  try {
    response =
        service_.ReadAllUserBuyBids(request_body.at(BDNames::bid_table_id));
    nlohmann::json status;
    status[ExtraJSONKeys::status] = ServerMessage::OK;
    response.emplace_back(status);
    return response;
  } catch (const std::exception &e) {
    ResponseError::Error(response, e.what());
    return response;
  }
}

nlohmann::json BidController::GetUserSellBids(
    const nlohmann::json &request_body) {
  nlohmann::json response;
  try {
    response =
        service_.ReadAllUserSellBids(request_body.at(BDNames::bid_table_id));
    nlohmann::json status;
    status[ExtraJSONKeys::status] = ServerMessage::OK;
    response.emplace_back(status);
    return response;
  } catch (const std::exception &e) {
    ResponseError::Error(response, e.what());
    return response;
  }
}

nlohmann::json BidController::UpdateBidQuantity(
    const nlohmann::json &request_body) {
  nlohmann::json response;
  try {
    service_.UpdateBidQuantity(request_body.at(BDNames::bid_table_id),
                               request_body.at(BDNames::bid_table_quantity));
    response = service_.ReadBid(request_body.at(BDNames::bid_table_id));
    response[ExtraJSONKeys::status] = ServerMessage::ResponseCode::OK;
    return response;
  } catch (const std::exception &e) {
    ResponseError::Error(response, e.what());
    return response;
  }
}

nlohmann::json BidController::UpdateBidRate(
    const nlohmann::json &request_body) {
  nlohmann::json response;
  try {
    response = service_.UpdateBidRate(request_body.at(BDNames::bid_table_id),
                                      request_body.at(BDNames::bid_table_rate));
    try {
      response.emplace_back(
          service_.ReadBid(request_body.at(BDNames::bid_table_id)));
    } catch (...) {
      {}
    }
    nlohmann::json status;
    status[ExtraJSONKeys::status] = ServerMessage::OK;
    response.emplace_back(status);
    return response;
  } catch (const std::exception &e) {
    ResponseError::Error(response, e.what());
    return response;
  }
}

nlohmann::json BidController::CancelBid(const nlohmann::json &request_body) {
  nlohmann::json response;
  try {
    service_.CancelBid(request_body.at(BDNames::bid_table_id),
                       request_body.at(BDNames::trader_id));
    response[ExtraJSONKeys::status] = ServerMessage::ResponseCode::OK;
    return response;
  } catch (const std::exception &e) {
    ResponseError::Error(response, e.what());
    return response;
  }
}
}  // namespace s21