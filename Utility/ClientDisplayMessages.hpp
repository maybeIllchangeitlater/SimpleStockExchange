#ifndef SIMPLESTOCKEXCHANGE_UTILITY_CLIENTDISPLAYMESSAGES_HPP
#define SIMPLESTOCKEXCHANGE_UTILITY_CLIENTDISPLAYMESSAGES_HPP

namespace s21 {
    struct ClientDisplayMessages {
        //bids
        constexpr static const char * bid_fulfilled = "Bid Fulfilled";
        constexpr static const char * bid_created = "Bid Created";
        constexpr static const char * bid_cancelled = "Bid Cancelled";
        constexpr static const char * bid_updated = "Bid Updated";
        constexpr static const char * table_status = "Status";
        constexpr static const char * updated_bid_status = "Updated";
        constexpr static const char * buy_bid_type = "Buy";
        constexpr static const char * sell_bid_type = "Sell";
        constexpr static const char * invalid_bid_type = "Invalid bid type";
        //general
        constexpr static const char * logged_in = "Logged in as ";
        constexpr static const char * registred = "Registered with username ";
        constexpr static const char * logged_out = "Signed out";
        constexpr static const char * account_del = "Account Deleted";
        constexpr static const char * namechange = "Username changed to ";
        //quotations
        constexpr static const char * daily_rate_for_usd = "Today's rate for USD is:";
        constexpr static const char * weekly_rate_for_usd = "This week rate for USD is:";
        constexpr static const char * monthly_rate_for_usd = "This month rate for USD is:";
        constexpr static const char * yearly_rate_for_usd = "This year rate for USD is:";
        constexpr static const char * no_transactions = "No transactions found";
        constexpr static const char * missing = "null";
        //user settings
        constexpr static const char * usd_balance = "usd balance: ";
        constexpr static const char * rub_balance = "rub balance: ";
    };
}

#endif //SIMPLESTOCKEXCHANGE_UTILITY_CLIENTDISPLAYMESSAGES_HPP
