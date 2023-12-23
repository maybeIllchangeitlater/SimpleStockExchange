#ifndef SIMPLESTOCKEXCHANGE_TRANSACTIONSERVICE_HPP
#define SIMPLESTOCKEXCHANGE_TRANSACTIONSERVICE_HPP

#include "../Repository/TransactionRepository.hpp"
#include "BalanceService.hpp"
#include "../Utility/Timestamper.hpp"

namespace s21{
    class TransactionService{
    public:
        TransactionService(TransactionRepository& repository, BalanceService &balance_service)
        : repository_(repository), balance_service_(balance_service) {}

        void MakeTransaction(const std::string &seller_id, const std::string &buyer_id,
                             const std::string &rate, const std::string &quantity){
            std::cout << "making transaction\n";
            repository_.CreateTransaction(seller_id, buyer_id,
                                          rate, quantity, Timestamper::GetTimestamp());
            balance_service_.DecreaseUsdIncreaseRubBalance(seller_id, rate, quantity);
            balance_service_.IncreaseUsdDecreaseRubBalance(buyer_id, rate, quantity);
        }

        nlohmann::json ReadTransaction(const std::string &transaction_id){
            return GenerateTransactionInfo(repository_.ReadTransaction(transaction_id)[0]);
        }

        nlohmann::json ReadAllUserSellTransactions(const std::string &user_id){
            auto transactions_info = repository_.ReadAllUserSellTransactions(user_id);
            nlohmann::json res;
            for(const auto& v: transactions_info){
                res += GenerateTransactionInfo(v);
            }
            return res;
        }

        nlohmann::json ReadAllUserBuyTransactions(const std::string &user_id){
            auto transactions_info = repository_.ReadAllUserBuyTransactions(user_id);
            nlohmann::json res;
            for(const auto& v: transactions_info){
                res += GenerateTransactionInfo(v);
            }
            return res;
        }
    private:
        nlohmann::json GenerateTransactionInfo(const pqxx::row & transaction_info){
            nlohmann::json transaction_json;
            transaction_json[BDNames::transaction_id_for_join] =
                    transaction_info[BDNames::transaction_id_for_join].as<std::string>();
            transaction_json[BDNames::joined_buyer_name] =
                    transaction_info[BDNames::joined_buyer_name].as<std::string>();
            transaction_json[BDNames::joined_seller_name] =
                    transaction_info[BDNames::joined_seller_name].as<std::string>();
            transaction_json[BDNames::transaction_table_rate] =
                    transaction_info[BDNames::transaction_table_rate].as<std::string>();
            transaction_json[BDNames::transaction_table_quantity] =
                    transaction_info[BDNames::transaction_table_quantity].as<std::string>();
            transaction_json[BDNames::transaction_table_create_update_time] =
                    transaction_info[BDNames::transaction_table_create_update_time].as<std::string>();
            return transaction_json;
        }
        TransactionRepository& repository_;
        BalanceService &balance_service_;
    };
} //s21

#endif //SIMPLESTOCKEXCHANGE_TRANSACTIONSERVICE_HPP
