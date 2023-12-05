#ifndef SIMPLESTOCKEXCHANGE_USERCONTROLLER_HPP
#define SIMPLESTOCKEXCHANGE_USERCONTROLLER_HPP

#include "../3rdParty/json.hpp"
#include "../Service/UserService.hpp"

namespace s21 {
    class UserController {
    public:
        explicit UserController(UserService &service) : service_(service){}
        bool AuthenticateUser(const std::string &user_name, const std::string &password){
            try{
               auto user_json = service_.GetUserByName(user_name);
               if(user_json[password] == password) {
                   //login
                   ;
               }
               else{
                   //not login
                   ;
               }
            }
        }
        bool RegisterUser(const std::string &id, const std::string &user_name,
                                    const std::string &password, const std::string &user_balance);
    private:
        UserService &service_;
    };
} //s21

#endif //SIMPLESTOCKEXCHANGE_USERCONTROLLER_HPP
