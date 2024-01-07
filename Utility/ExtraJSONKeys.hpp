#ifndef SIMPLESTOCKEXCHANGE_EXTRAJSONKEYS_HPP
#define SIMPLESTOCKEXCHANGE_EXTRAJSONKEYS_HPP

namespace s21 {
    struct ExtraJSONKeys {
        constexpr static const char * status = "status";
        constexpr static const char * message = "message";
        constexpr static const char * buy_transaction = "Bought ";
        constexpr static const char * sell_transaction = "Sold ";
        constexpr static const char * created_sell_bid_quantity = "Created bid to sell ";
        constexpr static const char * created_buy_bid_quantity = "Created bid to buy ";
        constexpr static const char * bid_transaction_rate = " USD with rate ";
        constexpr static const char * bid_creation_check = "Created bid to ";
        constexpr static const char * old_password = "old password";
    };
} //s21

#endif //SIMPLESTOCKEXCHANGE_EXTRAJSONKEYS_HPP
