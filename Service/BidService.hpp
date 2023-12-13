#ifndef SIMPLESTOCKEXCHANGE_BIDSERVICE_HPP
#define SIMPLESTOCKEXCHANGE_BIDSERVICE_HPP

#include "../Repository/BidRepository.hpp"
#include "../3rdParty/json.hpp"
#include "../Utility/Timestamper.hpp"
#include "../Utility/ServerMessage.hpp"



namespace s21{
    class BidService{
    public:
        enum BidType{
            BUYING,
            SELLING,
        };
        BidService(BidRepository &repository) : repository_(repository){}
        void CreateBid(const std::string &user_id, const std::string& rate,
                               const std::string &quantity, BidType bid_type){
            if(!ValidateRate(rate)){
                std::cout << "non-valid bid rate\n";
                throw std::logic_error(ServerMessage::server_message.at(ServerMessage::BID_BAD_RATE));
            }
            if(!ValidateQuantity(quantity)){
                std::cout << "non-valid bid count\n";
                throw std::logic_error(ServerMessage::server_message.at(ServerMessage::BID_BAD_QUANTITY));
            }
            std::cout << "Bid validated, trying to create\n";
            if(bid_type == BUYING){
                repository_.CreateBuyBid(user_id, rate, quantity, Timestamper::GetTimestamp());
            }else if(bid_type == SELLING){
                repository_.CreateSellBid(user_id, rate, quantity, Timestamper::GetTimestamp());
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
            return std::stoi(rate) > 0;
        }

        bool ValidateQuantity(const std::string &quantity){
            return std::stoi(quantity) > 0;
        }
        nlohmann::json GenerateBidInfo(const pqxx::row & bid_info){
            auto bid_id = bid_info["bid_id"].as<std::string>();
            std::cout << bid_id << "\n";
            auto seller = !bid_info["seller_username"].is_null()
                    ? bid_info["seller_username"].as<std::string>()
                    : "no seller yet";
            auto buyer = !bid_info["buyer_username"].is_null()
                    ? bid_info["buyer_username"].as<std::string>()
                    : "no buyer yer";
            auto quantity = bid_info["quantity"].as<std::string>();
            auto rate = bid_info["rate"].as<std::string>();
            auto time = bid_info["time"].as<std::string>();
            nlohmann::json bid_json;
            bid_json["bid_id"] = bid_id;
            bid_json["seller_username"] = seller;
            bid_json["buyer_username"] = buyer;
            bid_json["quantity"] = quantity;
            bid_json["rate"] = rate;
            bid_json["time"] = time;
            return bid_json;
        }
        BidRepository &repository_;
    };
} //s21

#endif //SIMPLESTOCKEXCHANGE_BIDSERVICE_HPP
