#include "BidService.hpp"

namespace s21 {

nlohmann::json BidService::CreateBid(const std::string &user_id,
                                     const std::string &rate,
                                     const std::string &quantity,
                                     s21::BidService::BidType bid_type) {
  if (!ValidateRate(rate)) {
    throw std::logic_error(
        ServerMessage::server_message.at(ServerMessage::BID_BAD_RATE));
  }
  if (!ValidateQuantity(quantity)) {
    throw std::logic_error(
        ServerMessage::server_message.at(ServerMessage::BID_BAD_QUANTITY));
  }
  return ProcessBid(user_id, rate, quantity, bid_type);
}

nlohmann::json BidService::UpdateBidRate(const std::string &bid_id,
                                         const std::string &rate) {
  if (!ValidateRate(rate)) {
    throw std::logic_error(
        ServerMessage::server_message.at(ServerMessage::BID_BAD_RATE));
  }
  repository_.UpdateBidRate(bid_id, rate, Timestamper::GetTimestamp());
  auto updated_bid = repository_.ReadBidRaw(bid_id);
  BidType type =
      updated_bid[0][BDNames::bid_table_seller_id].is_null() ? BUYING : SELLING;
  std::string user_id =
      updated_bid[0][BDNames::bid_table_seller_id].is_null()
          ? updated_bid[0][BDNames::bid_table_buyer_id].as<std::string>()
          : updated_bid[0][BDNames::bid_table_seller_id].as<std::string>();
  pqxx::result match = MatchBids(user_id, rate, type);
  nlohmann::json send_back;
  if (!match.empty()) {
    std::ptrdiff_t remaining_quantity = MakeTransactions(
        send_back, match, user_id, rate,
        updated_bid[0][BDNames::bid_table_quantity].as<std::string>(), type);
    if (remaining_quantity <= 0) {
      repository_.DeleteBid(bid_id);
    } else {
      repository_.UpdateBidQuantity(bid_id, std::to_string(remaining_quantity),
                                    Timestamper::GetTimestamp());
    }
  }
  return send_back;
}

void BidService::CancelBid(const std::string &bid_id,
                           const std::string &user_id) {
  {
    auto check = repository_.ReadBidRaw(bid_id);
    if (check[0][BDNames::bid_table_rate].is_null()) {
      throw std::runtime_error(
          ServerMessage::server_message.at(ServerMessage::BID_NOT_FOUND));
    }
    if (!check[0][BDNames::bid_table_buyer_id].is_null()) {
      auto buyer = check[0][BDNames::bid_table_buyer_id].as<std::string>();
      if (buyer == user_id) {
        repository_.DeleteBid(bid_id);
        return;
      }
    } else if (!check[0][BDNames::bid_table_seller_id].is_null()) {
      auto seller = check[0][BDNames::bid_table_seller_id].as<std::string>();
      if (seller == user_id) {
        repository_.DeleteBid(bid_id);
        return;
      }
    }
    throw std::runtime_error(
        ServerMessage::server_message.at(ServerMessage::BID_NOT_YOURS));
  }
}

nlohmann::json BidService::ProcessBid(const std::string &user_id,
                                      const std::string &rate,
                                      const std::string &quantity,
                                      BidType type) {
  nlohmann::json send_back;
  pqxx::result res = MatchBids(user_id, rate, type);
  if (!res.empty()) {
    auto remaining_quantity =
        MakeTransactions(send_back, res, user_id, rate, quantity, type);
    if (remaining_quantity > 0) {
      send_back +=
          MakeBid(user_id, rate, std::to_string(remaining_quantity), type);
    }
  } else {
    send_back +=
        MakeBid(user_id, rate, quantity,
                type);  // it fails here, some param is not checked for null
  }
  return send_back;
}

std::ptrdiff_t BidService::MakeTransactions(nlohmann::json &send_back,
                                            pqxx::result &matched_bids,
                                            const std::string &user_id,
                                            const std::string &rate,
                                            const std::string &quantity,
                                            BidType type) {
  auto remaining_quantity = std::stoll(quantity);
  for (const auto &row : matched_bids) {
    auto current_transaction =
        CompileCurrentTransaction(row, rate, quantity, type);

    if (type == BUYING) {
      transaction_service_.MakeTransaction(
          row[BDNames::bid_table_seller_id].as<std::string>(), user_id,
          row[BDNames::bid_table_rate].as<std::string>(),
          current_transaction[ExtraJSONKeys::buy_transaction]);
    } else {
      transaction_service_.MakeTransaction(
          user_id, row[BDNames::bid_table_buyer_id].as<std::string>(), rate,
          current_transaction[ExtraJSONKeys::sell_transaction]);
    }

    send_back += current_transaction;

    auto transaction_amount =
        type == BUYING
            ? std::stoll(std::string(
                  current_transaction[ExtraJSONKeys::buy_transaction]))
            : std::stoll(std::string(
                  current_transaction[ExtraJSONKeys::sell_transaction]));
    remaining_quantity -= transaction_amount;

    AlterAffectedBids(row, transaction_amount);

    if (remaining_quantity <= 0) break;
  }
  return remaining_quantity;
}

pqxx::result BidService::MatchBids(const std::string &user_id,
                                   const std::string &rate,
                                   s21::BidService::BidType type) {
  return type == SELLING ? repository_.MatchBuyBids(user_id, rate)
                         : repository_.MatchSellBids(user_id, rate);
}

nlohmann::json BidService::CompileCurrentTransaction(
    const pqxx::row &raw_match, const std::string &rate,
    const std::string &quantity, s21::BidService::BidType type) {
  nlohmann::json current_transaction;

  auto buyer_quant =
      type == BUYING
          ? std::stoll(quantity)
          : raw_match[BDNames::bid_table_quantity].as<std::ptrdiff_t>();
  auto seller_quant =
      type == SELLING
          ? std::stoll(quantity)
          : raw_match[BDNames::bid_table_quantity].as<std::ptrdiff_t>();
  if (type == BUYING) {
    current_transaction[ExtraJSONKeys::buy_transaction] =
        seller_quant <= buyer_quant ? std::to_string(seller_quant)
                                    : std::to_string(buyer_quant);
  } else {
    current_transaction[ExtraJSONKeys::sell_transaction] =
        seller_quant <= buyer_quant ? std::to_string(seller_quant)
                                    : std::to_string(buyer_quant);
  }

  current_transaction[ExtraJSONKeys::bid_transaction_rate] =
      type == BUYING ? raw_match[BDNames::bid_table_rate].as<std::string>()
                     : rate;
  return current_transaction;
}

void BidService::AlterAffectedBids(const pqxx::row &raw_match,
                                   std::ptrdiff_t transaction_amount) {
  auto second_party_remaining_quantity =
      raw_match[BDNames::bid_table_quantity].as<std::ptrdiff_t>() -
      transaction_amount;

  if (second_party_remaining_quantity <= 0) {
    repository_.DeleteBid(raw_match[BDNames::bid_table_id].as<std::string>());
  } else {
    repository_.UpdateBidQuantity(
        raw_match[BDNames::bid_table_id].as<std::string>(),
        std::to_string(second_party_remaining_quantity),
        Timestamper::GetTimestamp());
  }
}

nlohmann::json BidService::MakeBid(const std::string &user_id,
                                   const std::string &rate,
                                   const std::string &quantity,
                                   s21::BidService::BidType type) {
  nlohmann::json bid;
  bid[ExtraJSONKeys::bid_transaction_rate] = rate;
  if (type == BUYING) {
    bid[ExtraJSONKeys::created_buy_bid_quantity] = quantity;
    repository_.CreateBuyBid(user_id, rate, quantity,
                             Timestamper::GetTimestamp());
  } else {
    bid[ExtraJSONKeys::created_sell_bid_quantity] = quantity;
    repository_.CreateSellBid(user_id, rate, quantity,
                              Timestamper::GetTimestamp());
  }
  return bid;
}

nlohmann::json BidService::GenerateBidInfo(const pqxx::row &bid_info) {
  auto bid_id = bid_info[BDNames::bid_id_for_join].as<std::string>();
  auto seller = !bid_info[BDNames::joined_seller_name].is_null()
                    ? bid_info[BDNames::joined_seller_name].as<std::string>()
                    : BDNames::missing_seller;
  auto buyer = !bid_info[BDNames::joined_buyer_name].is_null()
                   ? bid_info[BDNames::joined_buyer_name].as<std::string>()
                   : BDNames::missing_buyer;
  auto quantity = bid_info[BDNames::bid_table_quantity].as<std::string>();
  auto rate = bid_info[BDNames::bid_table_rate].as<std::string>();
  auto time = bid_info[BDNames::bid_table_create_update_time].as<std::string>();
  nlohmann::json bid_json;
  bid_json[BDNames::bid_id_for_join] = bid_id;
  bid_json[BDNames::joined_seller_name] = seller;
  bid_json[BDNames::joined_buyer_name] = buyer;
  bid_json[BDNames::bid_table_quantity] = quantity;
  bid_json[BDNames::bid_table_rate] = rate;
  bid_json[BDNames::bid_table_create_update_time] = time;
  return bid_json;
}
}  // namespace s21