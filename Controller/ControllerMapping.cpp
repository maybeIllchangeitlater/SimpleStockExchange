#include "ControllerMapping.hpp"

namespace s21 {
const std::unordered_map<std::string, ControllerMapping::UserFunction>
    ControllerMapping::method_mapping_user{
        {"AuthenticateUser",
         [](UserController &c, const nlohmann::json &json) {
           return c.AuthenticateUser(json);
         }},
        {"GetUserBalance",
         [](UserController &c, const nlohmann::json &json) {
           return c.GetUserBalance(json);
         }},
        {"GetUserById",
         [](UserController &c, const nlohmann::json &json) {
           return c.GetUserById(json);
         }},
        {"GetUserByName",
         [](UserController &c, const nlohmann::json &json) {
           return c.GetUserByName(json);
         }},
        {"RegisterUser",
         [](UserController &c, const nlohmann::json &json) {
           return c.RegisterUser(json);
         }},
        {"DeleteUser",
         [](UserController &c, const nlohmann::json &json) {
           return c.DeleteUser(json);
         }},
        {"UpdateUserBalance",
         [](UserController &c, const nlohmann::json &json) {
           return c.UpdateUserBalance(json);
         }},
        {"UpdateUserName",
         [](UserController &c, const nlohmann::json &json) {
           return c.UpdateUserName(json);
         }},
        {"UpdateUserPassword",
         [](UserController &c, const nlohmann::json &json) {
           return c.UpdateUserPassword(json);
         }}};
const std::unordered_map<std::string, ControllerMapping::BidFunction>
    ControllerMapping::method_mapping_bid{
        {"CreateBid",
         [](BidController &c, const nlohmann::json &json) {
           return c.CreateBid(json);
         }},
        {"GetBid", [](BidController &c,
                      const nlohmann::json &json) { return c.GetBid(json); }},
        {"GetUserSellBids",
         [](BidController &c, const nlohmann::json &json) {
           return c.GetUserSellBids(json);
         }},
        {"GetUserBuyBids",
         [](BidController &c, const nlohmann::json &json) {
           return c.GetUserBuyBids(json);
         }},
        {"UpdateBidRate",
         [](BidController &c, const nlohmann::json &json) {
           return c.UpdateBidRate(json);
         }},
        {"UpdateBidQuantity",
         [](BidController &c, const nlohmann::json &json) {
           return c.UpdateBidQuantity(json);
         }},
        {"CancelBid", [](BidController &c, const nlohmann::json &json) {
           return c.CancelBid(json);
         }}};
const std::unordered_map<std::string, ControllerMapping::TransactionFunction>
    ControllerMapping::method_mapping_transaction{
        {"GetUserBuyTransactions",
         [](TransactionController &c, const nlohmann::json &json) {
           return c.GetUserBuyTransactions(json);
         }},
        {"GetUserSellTransactions",
         [](TransactionController &c, const nlohmann::json &json) {
           return c.GetUserSellTransactions(json);
         }},
        {"GetQuotations",
         [](TransactionController &c, const nlohmann::json &json) {
           return c.GetQuotations(json);
         }}};
}  // namespace s21