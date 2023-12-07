#include "ControllerMapping.hpp"

namespace s21{
    const std::unordered_map<std::string, std::any> ControllerMapping::method_mapping_user{
            {"GetUserById", [](UserController& c, std::string& str){ c.GetUserById(str); }},

//            {"GetUserById", &UserController::GetUserById},
//            {"GetUserByName", &UserController::GetUserByName},
//            {"RegisterUser", &UserController::RegisterUser},
//            {"DeleteUser", &UserController::DeleteUser},
//            {"UpdateUserBalance", &UserController::UpdateUserBalance},
//            {"UpdateUserName", &UserController::UpdateUserName},
//            {"UpdateUserPassword", &UserController::UpdateUserPassword},
    };
    const std::unordered_map<std::string, std::any> ControllerMapping::method_mapping_bid{
            {"CreateBid", &BidController::CreateBid},
            {"GetBid", &BidController::GetBid},
            {"GetUserBuyBids", &BidController::GetUserBuyBids},
            {"GetUserSellBids", &BidController::GetUserSellBids},
            {"UpdateBidRate", &BidController::UpdateBidRate},
            {"UpdateBidQuantity", &BidController::UpdateBidQuantity},
            {"CloseBid", &BidController::CloseBid},
            {"CancelBid", &BidController::CancelBid},
    };
    const std::unordered_map<std::string, std::any> ControllerMapping::method_mapping_transaction{
            {"MakeTransaction", &TransactionController::MakeTransaction},
            {"GetTransaction", &TransactionController::GetTransaction},
            {"GetUserBuyTransactions", &TransactionController::GetUserBuyTransactions},
            {"GetUserSellTransactions", &TransactionController::GetUserSellTransactions}
    };

    const std::unordered_map<char, std::unordered_map<std::string, std::any>> ControllerMapping::method_mapping{
            {'U', method_mapping_user},
            {'B', method_mapping_bid},
            {'T', method_mapping_transaction}
    };
}