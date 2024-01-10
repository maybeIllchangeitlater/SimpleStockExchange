#ifndef SIMPLESTOCKEXCHANGE_BIDSERVICE_HPP
#define SIMPLESTOCKEXCHANGE_BIDSERVICE_HPP

#include "../Repository/BidRepository.hpp"
#include "../Service/TransactionService.hpp"
#include "../Service/BalanceService.hpp"
#include "../Repository/BdNames.hpp"
#include "../3rdParty/json.hpp"
#include "../Utility/Timestamper.hpp"
#include "../Utility/ServerMessage.hpp"
#include "../Utility/ExtraJSONKeys.hpp"
#include "BidServiceInterface.hpp"



namespace s21{
    class BidService : public BidServiceInterface{
    public:

        BidService(BidRepository &repository, TransactionService &service)
        : repository_(repository), transaction_service_(service)
        {}

        ~BidService() override = default;

        nlohmann::json CreateBid(const std::string &user_id, const std::string& rate,
                               const std::string &quantity, BidType bid_type) override;

        void CancelBid(const std::string &bid_id, const std::string &user_id) override;

        nlohmann::json UpdateBidRate(const std::string &bid_id, const std::string &rate) override;

        void UpdateBidQuantity(const std::string &bid_id, const std::string &quantity) override{
            if(!ValidateQuantity(quantity)){
                throw std::logic_error(ServerMessage::server_message.at(ServerMessage::BID_BAD_QUANTITY));
            }
            repository_.UpdateBidQuantity(bid_id, quantity, Timestamper::GetTimestamp());
        }

        nlohmann::json ReadBid(const std::string &bid_id) override{
            return(GenerateBidInfo(repository_.ReadBid(bid_id)[0]));

        }
        nlohmann::json ReadAllUserSellBids(const std::string &user_id) override{
            auto transactions_info = repository_.ReadAllUserSellBids(user_id);
            nlohmann::json res;
            for(const auto& v: transactions_info){
                res += GenerateBidInfo(v);
            }
            return res;
        }

        nlohmann::json ReadAllUserBuyBids(const std::string &user_id) override{
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
        static nlohmann::json GenerateBidInfo(const pqxx::row & bid_info);
        std::ptrdiff_t MakeTransactions(nlohmann::json &send_back, pqxx::result &matched_bids,
                              const std::string &user_id, const std::string &rate,
                              const std::string &quantity, BidType type);
        static nlohmann::json CompileCurrentTransaction(const pqxx::row &raw_match, const std::string &rate,
                                                 const std::string &quantity, BidType type);
        void AlterAffectedBids(const pqxx::row &raw_match, std::ptrdiff_t transaction_amount);
        nlohmann::json ProcessBid(const std::string &user_id, const std::string &rate,
                                                const std::string &quantity, BidType bid);
        nlohmann::json MakeBid(const std::string &user_id, const std::string &rate,
                               const std::string &quantity, BidType type);
        pqxx::result MatchBids(const std::string &user_id, const std::string &rate, BidType type);

        BidRepository &repository_;
        TransactionService &transaction_service_;
    };
} //s21

#endif //SIMPLESTOCKEXCHANGE_BIDSERVICE_HPP
