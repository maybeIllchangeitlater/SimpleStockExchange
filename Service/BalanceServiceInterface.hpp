#ifndef SIMPLESTOCKEXCHANGE_SERVICE_BALANCESERVICEINTERFACE_HPP
#define SIMPLESTOCKEXCHANGE_SERVICE_BALANCESERVICEINTERFACE_HPP

#include <string>
#include "../3rdParty/json.hpp"

namespace s21{
    class BalanceServiceInterface{
    public:
        virtual ~BalanceServiceInterface() = default;
        virtual void CreateUserBalance(const std::string &user_id, const std::string &user_balance_usd,
                                       const std::string &user_balance_rub) = 0;
        virtual nlohmann::json GetUserBalance(const std::string &user_id) = 0;
        virtual void SetUserBalance(const std::string &user_id, const std::string &user_balance_usd,
                                    const std::string &user_balance_rub) = 0;
        virtual void IncreaseUsdDecreaseRubBalance(const std::string &user_id, const std::string &transaction_rate,
                                                   const std::string &transaction_quantity) = 0;
        virtual void DecreaseUsdIncreaseRubBalance(const std::string &user_id, const std::string &transaction_rate,
                                                   const std::string &transaction_quantity) = 0;
        virtual bool ValidateBalance(const std::string &balance) = 0;
    };
} //s21

#endif //SIMPLESTOCKEXCHANGE_SERVICE_BALANCESERVICEINTERFACE_HPP
