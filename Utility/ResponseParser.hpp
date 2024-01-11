#ifndef SIMPLESTOCKEXCHANGE_UTILITY_RESPONSEPARSER_HPP
#define SIMPLESTOCKEXCHANGE_UTILITY_RESPONSEPARSER_HPP

#include <string>
#include <algorithm>
#include "../Repository/BdNames.hpp"
#include "ExtraJSONKeys.hpp"

namespace s21 {
    class ResponseParser {
    public:
        ///checks if response contains error
        static bool CheckStatus(const std::string &msg) noexcept;
        ///checks if transactions were made during bid creation
        static bool CheckIfTransactionsWereMade(const std::string &msg) noexcept;
        ///clean ups jsontype serveresponse
        static std::string CleanServerResponse(const std::string &msg);
        ///cleans up jsontype bidcreatedresponse
        static std::string CleanBidCreatedResponse(std::string &&msg);
        ///cleans up jsontype detailedbidinfo
        static std::string ExtractDetailedBidInfoRaw(std::string &msg);
        ///cleans up jsontype transactioninfo
        static std::string ExtractTransactionInfoJson(std::string &msg);
        ///cleans up jsontype balance
        static std::string ExtractCleanBidUpdateResponse(std::string &msg);
        ///extracts [usd, rub] balance from jsontype string
        static std::pair<std::string, std::string> ExtractCleanBalance(std::string &&msg);

    };
} //s21

#endif //SIMPLESTOCKEXCHANGE_UTILITY_RESPONSEPARSER_HPP
