#include "TransactionService.hpp"

namespace s21{

    nlohmann::json TransactionService:GenerateTransactionInfo(const pqxx::row &transaction_info){
        nlohmann::json transaction_json;
        transaction_json[BDNames::transaction_id_for_join] =
                transaction_info[BDNames::transaction_id_for_join].as<std::string>();
        transaction_json[BDNames::joined_buyer_name] = transaction_info[BDNames::joined_buyer_name].is_null()
                                                       ? BDNames::deleted_account
                                                       : transaction_info[BDNames::joined_buyer_name].as<std::string>();
        transaction_json[BDNames::joined_seller_name] = transaction_info[BDNames::joined_seller_name].is_null()
                                                        ? BDNames::deleted_account
                                                        : transaction_info[BDNames::joined_seller_name].as<std::string>();
        transaction_json[BDNames::transaction_table_rate] =
                transaction_info[BDNames::transaction_table_rate].as<std::string>();
        transaction_json[BDNames::transaction_table_quantity] =
                transaction_info[BDNames::transaction_table_quantity].as<std::string>();
        transaction_json[BDNames::transaction_table_create_update_time] =
                transaction_info[BDNames::transaction_table_create_update_time].as<std::string>();
        return transaction_json;
    }

    nlohmann::json TransactionService::GetQuotations(const std::string &time_period) {
        auto transactions = repository_.GetTransactionsForLast(time_period);
        nlohmann::json transaction_info;
        double rate(0.0);
        size_t total_quantity(0);
        for(const auto &transaction : transactions){
            rate += transaction[BDNames::transaction_table_rate].as<double>()
                    * transaction[BDNames::transaction_table_quantity].as<size_t>();
            total_quantity += transaction[BDNames::transaction_table_quantity].as<size_t>();
        }
        transaction_info[ExtraJSONKeys::quotations] = rate/total_quantity;
        return transaction_info;
    }
}