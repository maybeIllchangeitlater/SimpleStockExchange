#ifndef SIMPLESTOCKEXCHANGE_USERSERVICE_HPP
#define SIMPLESTOCKEXCHANGE_USERSERVICE_HPP

#include "../Repository/UserRepository.hpp"
#include "../Utility/UUIDGenerator.hpp"
#include "../Utility/Encoder.hpp"
#include "../Utility/ServerMessage.hpp"
#include "../3rdParty/json.hpp"
#include <iostream>

namespace s21 {
    class UserService {
    public:
        explicit UserService(UserRepository &repository) :repository_(repository){}
        void CreateUser(const std::string &user_name, const std::string &password, const std::string &user_balance){
            if(!ValidateUserName(user_name)){
                throw std::logic_error(ServerMessage::server_message.at(ServerMessage::REGISTER_BAD_NAME));
            }
            if(!ValidatePassword(password)){
                throw std::logic_error(ServerMessage::server_message.at(ServerMessage::REGISTER_BAD_PASSWORD));
            }
            std::cout << "User Validated\n";
            repository_.CreateUser(UUIDGenerator::Generate(), user_name, Encoder::Encode(password), user_balance);
        }
        nlohmann::json GetUserByName(const std::string &user_name){
            return GenerateUserInfo(repository_.ReadUserByName(user_name));
        }
        nlohmann::json GetUserById(const std::string &user_id){
            return GenerateUserInfo(repository_.ReadUserById(user_id));
        }
        void UpdateUserName(const std::string &user_id, const std::string &new_name){
            if(!ValidateUserName(new_name)){
                throw std::logic_error(ServerMessage::server_message.at(ServerMessage::REGISTER_BAD_NAME));
            }
            repository_.UpdateUserName(user_id, new_name);
        }
        void UpdateUserBalance(const std::string &user_id, const std::string &new_balance){
            repository_.UpdateUserBalance(user_id, new_balance);
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
        bool ValidatePassword(const std::string &password){
            if(password.length() < 6) {
                return false;
            }
            bool has_uppercase = false;
            bool has_lowercase = false;
            bool has_symbols = false;
            for(const auto ch : password){
                if(std::isupper(ch)){
                    has_uppercase = true;
                }
                if(std::islower(ch)){
                    has_lowercase = true;
                }
                if(!std::isalnum(ch) || std::isdigit(ch)){
                    has_symbols = true;
                }
            }
            return has_uppercase && has_lowercase && has_symbols;
        }
        nlohmann::json GenerateUserInfo(const pqxx::result &user_info){
            std::string id = user_info[0]["id"].as<std::string>();
            std::string user_name = user_info[0]["username"].as<std::string>();
            std::string hashed_password = user_info[0]["password"].as<std::string>();
            std::string balance = user_info[0]["balance"].as<std::string>();
            nlohmann::json user_json;
            user_json["id"] = id;
            user_json["username"] = user_name;
            user_json["password"] = hashed_password;
            user_json["balance"] = balance;
            return user_json;
        }
        UserRepository &repository_;
    };
} //s21

#endif //SIMPLESTOCKEXCHANGE_USERSERVICE_HPP
