#ifndef SIMPLESTOCKEXCHANGE_BDNAMES_HPP
#define SIMPLESTOCKEXCHANGE_BDNAMES_HPP
#include <string>
namespace s21 {
    struct BDNames {
        constexpr static const char * user_table = "user_info";
        constexpr static const char * user_table_id = "id";
        constexpr static const char * user_table_user_name = "username";
        constexpr static const char * user_table_password = "password";
        constexpr static const char * user_table_balance = "balance";

        constexpr static const char * bid_table = "bid_info";
        constexpr static const char * bid_table_id = "id";
        constexpr static const char * bid_table_seller_id = "seller_id";
        constexpr static const char * bid_table_buyer_id = "buyer_id";
        constexpr static const char * bid_table_rate = "rate";
        constexpr static const char * bid_table_quantity = "quantity";
        constexpr static const char * bid_table_create_update_time = "time";
        constexpr static const char * bid_table_type = "type";

        constexpr static const char * transaction_table = "transaction_info";
        constexpr static const char * transaction_table_id = "id";
        constexpr static const char * transaction_table_seller_id = "seller_id";
        constexpr static const char * transaction_table_buyer_id = "buyer_id";
        constexpr static const char * transaction_table_rate = "rate";
        constexpr static const char * transaction_table_quantity = "quantity";
        constexpr static const char * transaction_table_create_update_time = "time";
    };
} //s21

#endif //SIMPLESTOCKEXCHANGE_BDNAMES_HPP
