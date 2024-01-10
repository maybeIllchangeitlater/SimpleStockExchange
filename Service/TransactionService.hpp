#ifndef SIMPLESTOCKEXCHANGE_TRANSACTIONSERVICE_HPP
#define SIMPLESTOCKEXCHANGE_TRANSACTIONSERVICE_HPP

#include "../Repository/TransactionRepository.hpp"
#include "BalanceService.hpp"
#include "../Utility/Timestamper.hpp"
#include "../Utility/ExtraJSONKeys.hpp"
#include "TransactionServiceInterface.hpp"

namespace s21{
    class TransactionService : public TransactionServiceInterface{
    public:
        TransactionService(TransactionRepository& repository, BalanceService &balance_service)
        : repository_(repository), balance_service_(balance_service) {}

        ~TransactionService() override = default;

        void MakeTransaction(const std::string &seller_id, const std::string &buyer_id,
                             const std::string &rate, const std::string &quantity) override{
            repository_.CreateTransaction(seller_id, buyer_id,
                                          rate, quantity, Timestamper::GetTimestamp());
            balance_service_.DecreaseUsdIncreaseRubBalance(seller_id, rate, quantity);
            balance_service_.IncreaseUsdDecreaseRubBalance(buyer_id, rate, quantity);
        }

        nlohmann::json ReadTransaction(const std::string &transaction_id) override{
            return GenerateTransactionInfo(repository_.ReadTransaction(transaction_id)[0]);
        }

        nlohmann::json ReadAllUserSellTransactions(const std::string &user_id) override{
            auto transactions_info = repository_.ReadAllUserSellTransactions(user_id);
            nlohmann::json res;
            for(const auto& v: transactions_info){
                res += GenerateTransactionInfo(v);
            }
            return res;
        }

        nlohmann::json ReadAllUserBuyTransactions(const std::string &user_id) override{
            auto transactions_info = repository_.ReadAllUserBuyTransactions(user_id);
            nlohmann::json res;
            for(const auto& v: transactions_info){
                res += GenerateTransactionInfo(v);
            }
            return res;
        }

        nlohmann::json GetQuotations(const std::string &time_period) override;
    private:
        nlohmann::json GenerateTransactionInfo(const pqxx::row & transaction_info);
        TransactionRepository& repository_;
        BalanceService &balance_service_;
    };
} //s21

#endif //SIMPLESTOCKEXCHANGE_TRANSACTIONSERVICE_HPP
