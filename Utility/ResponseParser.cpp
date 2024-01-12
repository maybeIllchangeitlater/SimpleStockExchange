#include "ResponseParser.hpp"

namespace s21 {
bool ResponseParser::CheckStatus(const std::string &msg) noexcept {
  return msg.find("OK") != std::string::npos;
}

std::string ResponseParser::CleanServerResponse(const std::string &msg) {
  bool error = msg.find("message") != std::string::npos;
  size_t json_start = error ? msg.find_last_of('{') : msg.find('{');

  size_t json_end = error ? msg.find('}') : msg.rfind('}');
  return msg.substr(json_start + 1, json_end - json_start - 1);
}

bool ResponseParser::CheckIfTransactionsWereMade(
    const std::string &msg) noexcept {
  return msg.find(s21::ExtraJSONKeys::buy_transaction) != std::string::npos ||
         msg.find(s21::ExtraJSONKeys::sell_transaction) != std::string::npos;
}

std::string ResponseParser::CleanBidCreatedResponse(std::string &&msg) {
  size_t first_part_start = msg.rfind(',');
  size_t second_part_start = msg.rfind('{');
  std::string first_part =
      msg.substr(first_part_start + 1, msg.rfind('}') - first_part_start - 1);
  std::string second_part =
      msg.substr(second_part_start + 1, msg.find(',') - second_part_start - 1);

  msg = (first_part + second_part + " RUB per 1 USD");
  msg.erase(std::remove_if(msg.begin(), msg.end(),
                           [](char c) { return c == '\"' || c == ':'; }),
            msg.end());

  return msg;
}

std::string ResponseParser::ExtractDetailedBidInfoRaw(std::string &msg) {
  if (msg.find(s21::BDNames::bid_table_create_update_time) ==
      std::string::npos) {
    return "";
  }
  size_t info_start = msg.rfind('{');
  size_t info_end = msg.rfind('}');
  auto detailed_info = msg.substr(info_start + 1, info_end - info_start - 1);
  msg.erase(info_start - 2, info_end);
  return detailed_info;
}

std::string ResponseParser::ExtractTransactionInfoJson(std::string &msg) {
  size_t json_start = msg.find('[');
  size_t json_end = msg.rfind(']');
  return msg.substr(json_start, json_end - json_start + 1);
}

std::string ResponseParser::ExtractCleanBidUpdateResponse(std::string &msg) {
  std::string msg_bid_type =
      msg.find(s21::BDNames::missing_buyer) != std::string::npos ? " sell "
                                                                 : " buy ";

  std::string msg_bid =
      msg.substr(msg.find(s21::BDNames::bid_id_for_join) + 8,
                 msg.find(',') - msg.find(s21::BDNames::bid_id_for_join) - 8);

  size_t quant_start_pos = msg.find(s21::BDNames::bid_table_quantity);
  std::string msg_quant = msg.substr(
      quant_start_pos, msg.find(',', quant_start_pos) - quant_start_pos);

  size_t rate_start_pos = msg.find(s21::BDNames::bid_table_rate);
  std::string msg_rate = msg.substr(
      rate_start_pos, msg.find(',', rate_start_pos) - rate_start_pos);

  std::string bid_update("Updated" + msg_bid_type + "bid :\n" + msg_bid +
                         "\nto " + msg_quant + " and " + msg_rate);
  bid_update.erase(std::remove(bid_update.begin(), bid_update.end(), '"'),
                   bid_update.end());
  return bid_update;
}

std::pair<std::string, std::string> ResponseParser::ExtractCleanBalance(
    std::string &&msg) {
  size_t rub_start = msg.find(s21::BDNames::balance_table_rub) +
                     std::string(s21::BDNames::balance_table_rub).length() +
                     2;  //:
  size_t usd_start = msg.find(s21::BDNames::balance_table_usd) +
                     std::string(s21::BDNames::balance_table_usd).length() +
                     2;  //:
  std::string usd_balance = msg.substr(usd_start, msg.find('}') - usd_start);
  std::string rub_balance = msg.substr(rub_start, msg.find(',') - rub_start);
  return std::make_pair(std::move(usd_balance), std::move(rub_balance));
}
}  // namespace s21
