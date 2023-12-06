#ifndef SIMPLESTOCKEXCHANGE_USERCONTROLLER_HPP
#define SIMPLESTOCKEXCHANGE_USERCONTROLLER_HPP

#include "../3rdParty/json.hpp"
#include "../Service/UserService.hpp"
#include "../HTTP/Http.hpp"
#include "../Utility/Encoder.hpp"

namespace s21 {
    class UserController {
    public:
        explicit UserController(UserService &service) : service_(service){}
        void AuthenticateUser(const nlohmann::json &request_body){
            if(service_.GetUserByName(request_body.at("username")).at("password")
                == Encoder::Encode(request_body.at("password"))){
            }else{
                throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::LOGIN_BAD_PASSWORD));
            }
        }
        void RegisterUser(const nlohmann::json &request_body){
            service_.CreateUser(request_body.at("username"), request_body.at("password"), request_body.at("balance"));
        }
        nlohmann::json GetUserById(const nlohmann::json &request_body){
            auto user_json = service_.GetUserById(request_body.at("id"));
            user_json.erase("password");
            return user_json;
        }
        nlohmann::json GetUserByName(const nlohmann::json &request_body){
            auto user_json = service_.GetUserByName(request_body.at("username"));
            user_json.erase("password");
            return user_json;
        }

        void UpdateUserName(const nlohmann::json &request_body){
            service_.UpdateUserName(request_body.at("id"), request_body.at("newname"));
        }

        void UpdateUserPassword(const nlohmann::json &request_body){
            service_.UpdateUserPassword(request_body.at("id"), request_body.at("newpassword"));
        }

        void UpdateUserBalance(const nlohmann::json &request_body){
            service_.UpdateUserBalance(request_body.at("id"), request_body.at("newbalance"));
        }

        void DeleteUser(const nlohmann::json &request_body){
            service_.DeleteUser(request_body.at("id"));
        }

    private:
        UserService &service_;
    };
} //s21

#endif //SIMPLESTOCKEXCHANGE_USERCONTROLLER_HPP
