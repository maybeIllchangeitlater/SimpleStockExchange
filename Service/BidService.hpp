#ifndef SIMPLESTOCKEXCHANGE_BIDSERVICE_HPP
#define SIMPLESTOCKEXCHANGE_BIDSERVICE_HPP

#include "../Repository/BidRepository.hpp"
#include "../Service/TransactionService.hpp"
#include "../Repository/BdNames.hpp"
#include "../3rdParty/json.hpp"
#include "../Utility/Timestamper.hpp"
#include "../Utility/ServerMessage.hpp"
#include "../Utility/ExtraJSONKeys.hpp"



namespace s21{
    class BidService{
    public:
        enum BidType{
            BUYING,
            SELLING,
        };
        BidService(BidRepository &repository, TransactionService &service)
        : repository_(repository), transaction_service_(service){}
        nlohmann::json CreateBid(const std::string &user_id, const std::string& rate,
                               const std::string &quantity, BidType bid_type){
            if(!ValidateRate(rate)){
                throw std::logic_error(ServerMessage::server_message.at(ServerMessage::BID_BAD_RATE));
            }
            if(!ValidateQuantity(quantity)){
                throw std::logic_error(ServerMessage::server_message.at(ServerMessage::BID_BAD_QUANTITY));
            }
            return bid_type == BUYING
                ? MakeBuyBidOrTransaction(user_id, rate, quantity)
                : MakeSellBidOrTransaction(user_id, rate, quantity);
        }
        void UpdateBidRate(const std::string &bid_id, const std::string &rate){
            if(!ValidateRate(rate)){
                throw std::logic_error(ServerMessage::server_message.at(ServerMessage::BID_BAD_RATE));
            }
            repository_.UpdateBidRate(bid_id, rate, Timestamper::GetTimestamp());
        }
        void UpdateBidQuantity(const std::string &bid_id, const std::string &quantity){
            if(!ValidateQuantity(quantity)){
                throw std::logic_error(ServerMessage::server_message.at(ServerMessage::BID_BAD_QUANTITY));
            }
            repository_.UpdateBidQuantity(bid_id, quantity, Timestamper::GetTimestamp());
        }

        void CloseBid(const std::string &bid_id){
            repository_.DeleteBid(bid_id);
        }

        void CancelBid(const std::string &bid_id, const std::string &user_id){
            auto check = repository_.ReadBidRaw(bid_id);
            if(check[0][BDNames::bid_table_rate].is_null()){
                throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::BID_NOT_FOUND));
            }
            if(!check[0][BDNames::bid_table_buyer_id].is_null()){
                auto buyer = check[0][BDNames::bid_table_buyer_id].as<std::string>();
                if(buyer == user_id){
                    repository_.DeleteBid(bid_id);
                    return;
                }
            }else if(!check[0][BDNames::bid_table_seller_id].is_null()){
                auto seller = check[0][BDNames::bid_table_seller_id].as<std::string>();
                if(seller == user_id){
                    repository_.DeleteBid(bid_id);
                    return;
                }
            }
            throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::BID_NOT_YOURS));
        }

        nlohmann::json ReadBid(const std::string &bid_id){
            return(GenerateBidInfo(repository_.ReadBid(bid_id)[0]));

        }
        nlohmann::json ReadAllUserSellBids(const std::string &user_id){
            auto transactions_info = repository_.ReadAllUserSellBids(user_id);
            nlohmann::json res;
            for(const auto& v: transactions_info){
                res += GenerateBidInfo(v);
            }
            return res;
        }
        nlohmann::json ReadAllUserBuyBids(const std::string &user_id){
            auto transactions_info = repository_.ReadAllUserBuyBids(user_id);
            nlohmann::json res;
            for(const auto& v: transactions_info){
                res += GenerateBidInfo(v);
            }
            return res;
        }
    private:

        bool ValidateRate(const std::string &rate){
            try {
                return std::stod(rate) > 0;
            }catch(...){
                return false;
            }
        }

        bool ValidateQuantity(const std::string &quantity){
            try {
                return std::stoi(quantity) > 0;
            }catch(...){
                return false;
            }
        }
        nlohmann::json GenerateBidInfo(const pqxx::row & bid_info){
            auto bid_id = bid_info[BDNames::bid_id_for_join].as<std::string>();
            std::cout << bid_id << "\n";
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
        nlohmann::json MakeBuyBidOrTransaction(const std::string &user_id, const std::string &rate, const std::string &quantity){
            nlohmann::json send_back;
            pqxx::result res;
            res = repository_.MatchSellBids(user_id, rate); ///first try to match buying bid with already existing selling bids
            auto remaining_quantity = std::stoll(quantity);
            if(!res.empty()){
                for(const auto& col : res){
                    nlohmann::json current_transaction;
                    auto buyer_quant = std::stoll(quantity);
                    auto seller_quant = col[BDNames::bid_table_quantity].as<long long>();
                    current_transaction[ExtraJSONKeys::buy_transaction] = seller_quant <= buyer_quant
                                                                           ? std::to_string(seller_quant)
                                                                           : std::to_string(buyer_quant);
                    current_transaction[ExtraJSONKeys::bid_transaction_rate] = col[BDNames::bid_table_rate]
                            .as<std::string>();
                    transaction_service_.MakeTransaction(col[BDNames::bid_table_seller_id].as<std::string>(),
                                                         user_id, col[BDNames::bid_table_rate].as<std::string>(),
                                                         current_transaction[ExtraJSONKeys::buy_transaction]);
                    send_back += current_transaction;
                    remaining_quantity -= seller_quant;
                    if(seller_quant <= buyer_quant){
                        repository_.DeleteBid(col[BDNames::bid_table_id].as<std::string>());
                    }else{
                        repository_.UpdateBidQuantity(col[BDNames::bid_table_id].as<std::string>(),
                                std::to_string(seller_quant - buyer_quant),
                                Timestamper::GetTimestamp());
                    }
                    if(remaining_quantity <= 0)
                        break;
                }
                if(remaining_quantity > 0){
                    nlohmann::json current_transaction;
                    current_transaction[ExtraJSONKeys::created_buy_bid_quantity] = std::to_string(remaining_quantity);
                    current_transaction[ExtraJSONKeys::bid_transaction_rate] = rate;
                    send_back += current_transaction;
                    repository_.CreateBuyBid(user_id, rate, std::to_string(remaining_quantity), Timestamper::GetTimestamp());
                }
            }else{
                nlohmann::json current_transaction;
                current_transaction[ExtraJSONKeys::created_buy_bid_quantity] = std::to_string(remaining_quantity);
                current_transaction[ExtraJSONKeys::bid_transaction_rate] = rate;
                send_back += current_transaction;
                repository_.CreateBuyBid(user_id, rate, quantity, Timestamper::GetTimestamp());
            }
            return send_back;
        }
        nlohmann::json MakeSellBidOrTransaction(const std::string &user_id, const std::string &rate, const std::string &quantity){
            nlohmann::json send_back;
            pqxx::result res;
            res = repository_.MatchBuyBids(user_id, rate);
            auto remaining_quantity = std::stoll(quantity);
            if(!res.empty()){
                for(const auto& col : res){
                    nlohmann::json current_transaction;
                    auto seller_quant = std::stoll(quantity);
                    auto buyer_quant = col[BDNames::bid_table_quantity].as<long long>();
                    current_transaction[ExtraJSONKeys::sell_transaction] = seller_quant <= buyer_quant
                            ? std::to_string(seller_quant)
                            : std::to_string(buyer_quant);
                    current_transaction[ExtraJSONKeys::bid_transaction_rate] = col[BDNames::bid_table_rate].as<std::string>();
                    transaction_service_.MakeTransaction(col[BDNames::bid_table_buyer_id].as<std::string>(),
                                                         user_id, col[BDNames::bid_table_rate].as<std::string>(),
                                                         current_transaction[ExtraJSONKeys::sell_transaction]);
                    send_back += current_transaction;
                    remaining_quantity -= buyer_quant;
                    if(buyer_quant <= seller_quant){
                        repository_.DeleteBid(col[BDNames::bid_table_id].as<std::string>());
                    }else {
                        repository_.UpdateBidQuantity(col[BDNames::bid_table_id].as<std::string>(),
                                                      std::to_string(buyer_quant - seller_quant),
                                                      Timestamper::GetTimestamp());
                    }
                    if(remaining_quantity <= 0)
                        break;
                }
                if(remaining_quantity > 0){
                    nlohmann::json current_transaction;
                    current_transaction[ExtraJSONKeys::created_sell_bid_quantity] = std::to_string(remaining_quantity);
                    current_transaction[ExtraJSONKeys::bid_transaction_rate] = rate;
                    send_back += current_transaction;
                    repository_.CreateSellBid(user_id, rate, std::to_string(remaining_quantity), Timestamper::GetTimestamp());
                }
            }else{
                nlohmann::json current_transaction;
                current_transaction[ExtraJSONKeys::created_sell_bid_quantity] = std::to_string(remaining_quantity);
                current_transaction[ExtraJSONKeys::bid_transaction_rate] = rate;
                send_back += current_transaction;
                repository_.CreateSellBid(user_id, rate, quantity, Timestamper::GetTimestamp());
            }
            return send_back;
        }
        BidRepository &repository_;
        TransactionService &transaction_service_;

    };
} //s21

#endif //SIMPLESTOCKEXCHANGE_BIDSERVICE_HPP
