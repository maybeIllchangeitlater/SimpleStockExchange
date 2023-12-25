#ifndef SIMPLESTOCKEXCHANGE_USERSERVICE_HPP
#define SIMPLESTOCKEXCHANGE_USERSERVICE_HPP

#include "../Repository/UserRepository.hpp"
#include "../Repository/BdNames.hpp"
#include "../Utility/UUIDGenerator.hpp"
#include "../Utility/Encoder.hpp"
#include "../Utility/ServerMessage.hpp"
#include "../3rdParty/json.hpp"
#include "../Service/BalanceService.hpp"
#include <iostream>

namespace s21 {
    class UserService {
    public:
        explicit UserService(UserRepository &repository, BalanceService &balance_service)
        : repository_(repository), balance_service_(balance_service){}

        void CreateUser(const std::string &user_name, const std::string &password,
                        const std::string &user_balance_usd, const std::string &user_balance_rub);

        nlohmann::json GetUserByName(const std::string &user_name){
            auto result_json = GenerateUserInfo(repository_.ReadUserByName(user_name));
            auto balance_json = balance_service_.GetUserBalance(result_json[BDNames::user_table_id]);
            result_json[BDNames::balance_table_usd] = balance_json[BDNames::balance_table_usd];
            result_json[BDNames::balance_table_rub] = balance_json[BDNames::balance_table_rub];
            return result_json;
        }

        nlohmann::json GetUserById(const std::string &user_id){
            auto result_json = GenerateUserInfo(repository_.ReadUserById(user_id));
            auto balance_json = balance_service_.GetUserBalance(user_id);
            result_json[BDNames::balance_table_usd] = balance_json[BDNames::balance_table_usd];
            result_json[BDNames::balance_table_rub] = balance_json[BDNames::balance_table_rub];
            return result_json;
        }

        void UpdateUserName(const std::string &user_id, const std::string &new_name){
            if(!ValidateUserName(new_name)){
                throw std::logic_error(ServerMessage::server_message.at(ServerMessage::REGISTER_BAD_NAME));
            }
            repository_.UpdateUserName(user_id, new_name);
        }

        void UpdateUserBalance(const std::string &user_id, const std::string &new_balance_usd,
                               const std::string &new_balance_rub){
            if(!balance_service_.ValidateBalance(new_balance_rub)
            || !balance_service_.ValidateBalance(new_balance_usd)){
                throw std::logic_error(ServerMessage::server_message.at(ServerMessage::BALANCE_BAD_BALANCE));
            }
            balance_service_.SetUserBalance(user_id, new_balance_usd, new_balance_rub);
        }

        void UpdateUserPassword(const std::string &user_id, const std::string &new_password){
            if(!ValidatePassword(new_password)){
                throw std::logic_error(ServerMessage::server_message.at(ServerMessage::REGISTER_BAD_PASSWORD));
            }
            repository_.UpdateUserPassword(user_id, Encoder::Encode(new_password));
        }
        void DeleteUser(const std::string &user_id){
            repository_.DeleteUser(user_id);
        }

    private:
        bool ValidateUserName(const std::string &user_name){
            return user_name.length() > 3;
        }
        bool ValidatePassword(const std::string &password);

        nlohmann::json GenerateUserInfo(const pqxx::result &user_info){
            nlohmann::json user_json;
            user_json[BDNames::user_table_id] = user_info[0][BDNames::user_table_id].as<std::string>();
            user_json[BDNames::user_table_user_name] = user_info[0][BDNames::user_table_user_name].as<std::string>();
            user_json[BDNames::user_table_password] = user_info[0][BDNames::user_table_password].as<std::string>();
            return user_json;
        }
        UserRepository &repository_;
        BalanceService &balance_service_;
    };
} //s21

#endif //SIMPLESTOCKEXCHANGE_USERSERVICE_HPP
