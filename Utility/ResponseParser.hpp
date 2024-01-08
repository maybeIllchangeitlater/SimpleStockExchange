#ifndef SIMPLESTOCKEXCHANGE_UTILITY_RESPONSEPARSER_HPP
#define SIMPLESTOCKEXCHANGE_UTILITY_RESPONSEPARSER_HPP

#include <string>
#include "../Repository/BdNames.hpp"
#include "ExtraJSONKeys.hpp"

namespace s21 {
    class ResponseParser {
    public:
        static bool CheckStatus(const std::string &msg) noexcept;
        static bool CheckIfTransactionsWereMade(const std::string &msg) noexcept;
        static std::string CleanServerResponse(const std::string &msg);
        static std::string CleanBidCreatedResponse(std::string &&msg);
        static std::string ExtractDetailedBidInfoRaw(std::string &msg);
        static std::string ExtractTransactionInfoJson(std::string &msg);
        static std::string ExtractCleanBidUpdateResponse(std::string &msg);
        ///usd, rub
        static std::pair<std::string, std::string> ExtractCleanBalance(std::string &&msg);

    };
} //s21

#endif //SIMPLESTOCKEXCHANGE_UTILITY_RESPONSEPARSER_HPP
