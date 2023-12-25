#include "TransactionService.hpp"

namespace s21{

    nlohmann::json TransactionService::GenerateTransactionInfo(const pqxx::row &transaction_info){
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
}