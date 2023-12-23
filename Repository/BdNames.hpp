#ifndef SIMPLESTOCKEXCHANGE_BDNAMES_HPP
#define SIMPLESTOCKEXCHANGE_BDNAMES_HPP
#include <string>
namespace s21 {
    struct BDNames {
        constexpr static const char * user_table = "user_info";
        constexpr static const char * user_table_id = "id";
        constexpr static const char * user_table_user_name = "username";
        constexpr static const char * user_table_password = "password";

        constexpr static const char * balance_table = "balance_info";
        constexpr static const char * balance_table_user_id = "user_id";
        constexpr static const char * balance_table_usd = "usd_balance";
        constexpr static const char * balance_table_rub = "rub_balance";

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

        constexpr static const char * joined_seller_name = "seller_username";
        constexpr static const char * joined_buyer_name = "buyer_username";
        constexpr static const char * bid_id_for_join = "bid_id";
        constexpr static const char * transaction_id_for_join = "transaction_id";
        constexpr static const char * trader_id = "trader_id";

        constexpr static const char * missing_buyer = "no buyer yet";
        constexpr static const char * missing_seller = "no seller yet";
    };
} //s21

#endif //SIMPLESTOCKEXCHANGE_BDNAMES_HPP
