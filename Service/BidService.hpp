#ifndef SIMPLESTOCKEXCHANGE_BIDSERVICE_HPP
#define SIMPLESTOCKEXCHANGE_BIDSERVICE_HPP

#include "../Repository/BidRepository.hpp"
#include "../Repository/BdNames.hpp"
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
            repository_.DeleteBid(bid_id);
        }

        void CancelBid(const std::string &bid_id, const std::string &user_id){
            std::cout << "trying to cancel bid :" << bid_id << "\n";
            auto check = repository_.ReadBidRaw(bid_id);
            if(check[0][BDNames::bid_table_rate].is_null()){
                std::cout << "bid doesnt exist\n";
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
            std::cout << "Cannot delete bid that isnt yours\n";
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
            return std::stod(rate) > 0;
        }

        bool ValidateQuantity(const std::string &quantity){
            return std::stoi(quantity) > 0;
        }
        nlohmann::json GenerateBidInfo(const pqxx::row & bid_info){
            auto bid_id = bid_info[BDNames::bid_id_for_join].as<std::string>();
            std::cout << bid_id << "\n";
            auto seller = !bid_info[BDNames::joined_seller_name].is_null()
                    ? bid_info[BDNames::joined_seller_name].as<std::string>()
                    : "no seller yet";
            auto buyer = !bid_info[BDNames::joined_buyer_name].is_null()
                    ? bid_info[BDNames::joined_buyer_name].as<std::string>()
                    : "no buyer yet";
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
        BidRepository &repository_;
    };
} //s21

#endif //SIMPLESTOCKEXCHANGE_BIDSERVICE_HPP
