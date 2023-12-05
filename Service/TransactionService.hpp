#ifndef SIMPLESTOCKEXCHANGE_TRANSACTIONSERVICE_HPP
#define SIMPLESTOCKEXCHANGE_TRANSACTIONSERVICE_HPP

#include "../Repository/TransactionRepository.hpp"
#include "../Utility/UUIDGenerator.hpp"
#include "../Utility/Timestamper.hpp"

namespace s21{
    class TransactionService{
    public:
        TransactionService(TransactionRepository& repository) : repository_(repository) {}
        void MakeTransaction(const std::string &seller_id, const std::string &buyer_id,
                             const std::string &rate, const std::string &quantity){
            repository_.CreateTransaction(UUIDGenerator::Generate(), seller_id, buyer_id,
                                          rate, quantity, Timestamper::GetTimestamp());
        }
        nlohmann::json ReadTransaction(const std::string &transaction_id){
            return GenerateTransactionInfo(repository_.ReadTransaction(transaction_id)[0]);
        }
        std::vector<nlohmann::json> ReadAllUserSellTransactions(const std::string &user_id){
            auto transactions_info = repository_.ReadAllUserSellTransactions(user_id);
            std::vector<nlohmann::json> res;
            res.reserve(transactions_info.size());
            for(const auto& v: transactions_info){
                res.emplace_back(GenerateTransactionInfo(v));
            }
            return res;
        }
        std::vector<nlohmann::json> ReadAllUserBuyTransactions(const std::string &user_id){
            auto transactions_info = repository_.ReadAllUserBuyTransactions(user_id);
            std::vector<nlohmann::json> res;
            res.reserve(transactions_info.size());
            for(const auto& v: transactions_info){
                res.emplace_back(GenerateTransactionInfo(v));
            }
            return res;
        }
    private:
        nlohmann::json GenerateTransactionInfo(const pqxx::row & transaction_info){
            std::string id = transaction_info["id"].as<std::string>();
            std::string buyer = transaction_info["buyer name"].as<std::string>();
            std::string seller = transaction_info["seller name"].as<std::string>();
            std::string rate = transaction_info["rate"].as<std::string>();
            std::string quantity = transaction_info["quantity"].as<std::string>();
            std::string time = transaction_info["time"].as<std::string>();
            nlohmann::json transaction_json;
            transaction_json["id"] = id;
            transaction_json["buyer"] = buyer;
            transaction_json["seller"] = seller;
            transaction_json["rate"] = rate;
            transaction_json["quantity"] = quantity;
            transaction_json["time"] = time;
            return transaction_json;
        }
        TransactionRepository& repository_;
    };
} //s21

#endif //SIMPLESTOCKEXCHANGE_TRANSACTIONSERVICE_HPP
