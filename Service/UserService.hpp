#ifndef SIMPLESTOCKEXCHANGE_USERSERVICE_HPP
#define SIMPLESTOCKEXCHANGE_USERSERVICE_HPP

#include "../Repository/UserRepository.hpp"
#include "../3rdParty/json.hpp"

namespace s21 {
    class UserService {
    public:
        explicit UserService(UserRepository &repository) :repository_(repository){}
        nlohmann::json GetUserByName(const std::string &user_name){
            auto user_info = repository_.ReadUserByName(user_name);
            std::string id = user_info[0]["id"].as<std::string>();
            std::string hashed_password = user_info[0]["password"].as<std::string>();
            std::string balance = user_info[0]["balance"].as<std::string>();
            nlohmann::json user_json;
            user_json["id"] = id;
            user_json["user_name"] = user_name;
            user_json["password"] = hashed_password;
            user_json["balance"] = balance;
            return user_json;
        }

    private:
        UserRepository &repository_;
    };
} //s21

#endif //SIMPLESTOCKEXCHANGE_USERSERVICE_HPP
