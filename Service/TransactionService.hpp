#ifndef SIMPLESTOCKEXCHANGE_TRANSACTIONSERVICE_HPP
#define SIMPLESTOCKEXCHANGE_TRANSACTIONSERVICE_HPP

#include "../Repository/TransactionRepository.hpp"
#include "../Utility/Timestamper.hpp"

namespace s21{
    class TransactionService{
    public:
        TransactionService(TransactionRepository& repository) : repository_(repository) {}

        void MakeTransaction(const std::string &seller_id, const std::string &buyer_id,
                             const std::string &rate, const std::string &quantity){
            std::cout << "making transaction\n";
            repository_.CreateTransaction(seller_id, buyer_id,
                                          rate, quantity, Timestamper::GetTimestamp());
        }
        nlohmann::json ReadTransaction(const std::string &transaction_id){
            return GenerateTransactionInfo(repository_.ReadTransaction(transaction_id)[0]);
        }
        nlohmann::json ReadAllUserSellTransactions(const std::string &user_id){
            auto transactions_info = repository_.ReadAllUserSellTransactions(user_id);
            nlohmann::json res;
            for(const auto& v: transactions_info){
                res.emplace_back(GenerateTransactionInfo(v));
            }
            return res;
        }
        nlohmann::json ReadAllUserBuyTransactions(const std::string &user_id){
            auto transactions_info = repository_.ReadAllUserBuyTransactions(user_id);
            nlohmann::json res;
            for(const auto& v: transactions_info){
                res.emplace_back(GenerateTransactionInfo(v));
            }
            return res;
        }
    private:
        nlohmann::json GenerateTransactionInfo(const pqxx::row & transaction_info){
            auto id = transaction_info["id"].as<std::string>();
            auto buyer = transaction_info["buyer name"].as<std::string>();
            auto seller = transaction_info["seller name"].as<std::string>();
            auto rate = transaction_info["rate"].as<std::string>();
            auto quantity = transaction_info["quantity"].as<std::string>();
            auto time = transaction_info["time"].as<std::string>();
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
