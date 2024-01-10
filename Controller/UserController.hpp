#ifndef SIMPLESTOCKEXCHANGE_USERCONTROLLER_HPP
#define SIMPLESTOCKEXCHANGE_USERCONTROLLER_HPP

#include "../3rdParty/json.hpp"
#include "../Service/UserServiceInterface.hpp"
#include "../HTTP/Http.hpp"
#include "../Utility/Encoder.hpp"
#include "../Repository/BdNames.hpp"
#include "../Utility/ExtraJSONKeys.hpp"
#include "../Utility/ResponseError.hpp"

namespace s21 {
    class UserController {
    public:

        explicit UserController(UserServiceInterface &service) : service_(service){}

        nlohmann::json AuthenticateUser(const nlohmann::json &request_body);

        nlohmann::json RegisterUser(const nlohmann::json &request_body);

        nlohmann::json GetUserById(const nlohmann::json &request_body);

        nlohmann::json GetUserByName(const nlohmann::json &request_body);

        nlohmann::json GetUserBalance(const nlohmann::json &request_body);

        nlohmann::json UpdateUserName(const nlohmann::json &request_body);

        nlohmann::json UpdateUserPassword(const nlohmann::json &request_body);

        nlohmann::json UpdateUserBalance(const nlohmann::json &request_body);

        nlohmann::json DeleteUser(const nlohmann::json &request_body);

    private:
        UserServiceInterface &service_;
    };
} //s21

#endif //SIMPLESTOCKEXCHANGE_USERCONTROLLER_HPP
