#ifndef SIMPLESTOCKEXCHANGE_TRANSACTIONSERVICE_HPP
#define SIMPLESTOCKEXCHANGE_TRANSACTIONSERVICE_HPP

#include "../Repository/BidRepository.hpp"
#include "../3rdParty/json.hpp"
#include "../Utility/UUIDGenerator.hpp"
#include "../Utility/Timestamper.hpp"
#include "../Utility/ServerMessage.hpp"



namespace s21{
    class BidService{
    public:
        enum BidType{
            kBuying,
            kSelling,
        };
        BidService(BidRepository &repository) : repository_(repository){}
        void CreateBid(const std::string &user_id, const std::string& rate,
                               const std::string &quantity, BidType bid_type){
            if(!ValidateRate(rate)){
                throw std::logic_error(ServerMessage::server_message.at(ServerMessage::BID_BAD_RATE));
            }
            if(!ValidateQuantity(quantity)){
                throw std::logic_error(ServerMessage::server_message.at(ServerMessage::BID_BAD_QUANTITY));
            }
            if(bid_type == kBuying){
                repository_.CreateBuyBid(UUIDGenerator::Generate(), user_id, rate, quantity, Timestamper::GetTimestamp());
            }else if(bid_type == kSelling){
                repository_.CreateSellBid(UUIDGenerator::Generate(), user_id, rate, quantity, Timestamper::GetTimestamp());
            }
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
            repository_.DeleteFinishedBid(bid_id);
        }

        void CancelBid(const std::string &bid_id){
            repository_.DeleteUnfinishedBid(bid_id);
        }

        nlohmann::json ReadBid(const std::string &bid_id){
            return(GenerateBidInfo(repository_.ReadBid(bid_id)[0]));

        }
        std::vector<nlohmann::json> ReadAllUserSellBids(const std::string &user_id){
            auto transactions_info = repository_.ReadAllUserSellBids(user_id);
            std::vector<nlohmann::json> res;
            res.reserve(transactions_info.size());
            for(const auto& v: transactions_info){
                res.emplace_back(GenerateBidInfo(v));
            }
            return res;
        }
        std::vector<nlohmann::json> ReadAllUserBuyBids(const std::string &user_id){
            auto transactions_info = repository_.ReadAllUserBuyBids(user_id);
            std::vector<nlohmann::json> res;
            res.reserve(transactions_info.size());
            for(const auto& v: transactions_info){
                res.emplace_back(GenerateBidInfo(v));
            }
            return res;
        }
    private:

        bool ValidateRate(const std::string &rate){
            return std::stoi(rate) > 0;
        }

        bool ValidateQuantity(const std::string &quantity){
            return std::stoi(quantity) > 0;
        }
        nlohmann::json GenerateBidInfo(const pqxx::row & bid_info){
            std::string bid_id = bid_info["bid_id"].as<std::string>();
            std::string seller = bid_info["seller username"].as<std::string>();
            std::string buyer = bid_info["buyer username"].as<std::string>();
            std::string quantity = bid_info["quantity"].as<std::string>();
            std::string rate = bid_info["rate"].as<std::string>();
            std::string time = bid_info["time"].as<std::string>();
            nlohmann::json bid_json;
            bid_json["bid id"] = bid_id;
            bid_json["seller username"] = seller;
            bid_json["buyer username"] = buyer;
            bid_json["quantity"] = quantity;
            bid_json["rate"] = rate;
            bid_json["time"] = time;
            return bid_json;
        }
        BidRepository &repository_;
    };
} //s21

#endif //SIMPLESTOCKEXCHANGE_TRANSACTIONSERVICE_HPP
