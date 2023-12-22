#ifndef SIMPLESTOCKEXCHANGE_BALANCESERVICE_HPP
#define SIMPLESTOCKEXCHANGE_BALANCESERVICE_HPP

#include "../Repository/BalanceRepository.hpp"
#include "../Repository/BdNames.hpp"
#include "../3rdParty/json.hpp"


namespace s21 {
    class BalanceService {
    public:
        explicit BalanceService(BalanceRepository &balance_repository) : balance_repository_(balance_repository){}
        nlohmann::json GetUserBalance(const std::string &user_id){
            nlohmann::json res;
            auto balance_info = balance_repository_.GetUserBalance(user_id);
            res[BDNames::balance_table_usd] = balance_info[0][BDNames::balance_table_usd].as<std::string>();
            res[BDNames::balance_table_rub] = balance_info[0][BDNames::balance_table_usd].as<std::string>();
            return res;
        }

        void SetUserBalance(const std::string &user_id, const std::string &user_balance_usd,
                            const std::string &user_balance_rub){
            balance_repository_.SetUserBalance(user_id, user_balance_usd, user_balance_rub);
        }

        bool ValidateBalance(const std::string &balance){
            try{
                std::stod(balance);
                return true;
            }catch(...){
                return false;
            }
        }
    private:
        BalanceRepository &balance_repository_;
    };
}

#endif //SIMPLESTOCKEXCHANGE_BALANCESERVICE_HPP
