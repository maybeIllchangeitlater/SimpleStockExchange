#include "UserController.hpp"

namespace s21{
    nlohmann::json UserController::AuthenticateUser(const nlohmann::json &request_body) {
        std::cout << "Login attempt from " << request_body.at("username") << "\n";
        nlohmann::json response;
        try {
            auto user = service_.GetUserByName(request_body.at(BDNames::user_table_user_name));
            if (user.at(
                    BDNames::user_table_password)
                == Encoder::Encode(request_body.at(BDNames::user_table_password))) {
                std::cout << "user password is " << user.at(
                        BDNames::user_table_password) << " vs passed password " << Encoder::Encode(request_body.at(BDNames::user_table_password));
                response["status"] = ServerMessage::ResponseCode::OK;
                response[BDNames::user_table_id] = user.at(BDNames::user_table_id);
                return response;
            } else {
                throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::LOGIN_BAD_PASSWORD));
            }
        }catch(const std::exception &e){
            ResponseError(response, e.what());
            return response;
        }
    }

    nlohmann::json UserController::RegisterUser(const nlohmann::json &request_body){
        nlohmann::json response;
        try {
            auto check_if_exists = service_.GetUserByName(request_body.at(BDNames::user_table_user_name));
            if(!check_if_exists.empty()) {
                ResponseError(response, ServerMessage::server_message.at(ServerMessage::REGISTER_NOT_UNIQUE_NAME));
                return response;
            }
        }catch(const std::exception &e){
            try {
                service_.CreateUser(request_body.at(BDNames::user_table_user_name),
                                    request_body.at(BDNames::user_table_password),
                                    request_body.at(BDNames::user_table_balance));
                auto user = service_.GetUserByName(request_body.at(BDNames::user_table_user_name));
                response["status"] = ServerMessage::ResponseCode::OK;
                response[BDNames::user_table_id] = user.at(BDNames::user_table_id);
                return response;
            }catch(const std::exception &e){
                ResponseError(response, e.what());
                return response;
            }
        }
        return response; //for compiler warnings
    }

    nlohmann::json UserController::GetUserById(const nlohmann::json &request_body){
        nlohmann::json response;
        try {
            response = service_.GetUserById(request_body.at(BDNames::user_table_id));
            response.erase(BDNames::user_table_password);
            response["status"] = ServerMessage::ResponseCode::OK;
            return response;
        }catch(const std::exception &e){
            ResponseError(response, e.what());
            return response;
        }
    }

    nlohmann::json UserController::GetUserBalance(const nlohmann::json &request_body) {
        nlohmann::json response;
        try {
            std::cout << request_body.at(BDNames::user_table_id) << "\n";
            auto tmp = GetUserById(request_body.at(BDNames::user_table_id));
            std::cout << "requested user: " << tmp.at(BDNames::user_table_user_name)
            << "  " << tmp.at(BDNames::user_table_balance) << "\n";
            response[BDNames::user_table_balance] = tmp[BDNames::user_table_balance];
            response["status"] = ServerMessage::ResponseCode::OK;
            return response;
        } catch (const std::exception &e) {
            ResponseError(response, e.what());
            return response;
        }
    }

    nlohmann::json UserController::GetUserByName(const nlohmann::json &request_body){
        nlohmann::json response;
        try {
            response = service_.GetUserByName(request_body.at(BDNames::user_table_user_name));
            response.erase(BDNames::user_table_password);
            response["status"] = ServerMessage::ResponseCode::OK;
            return response;
        }catch(const std::exception &e){
            ResponseError(response, e.what());
            return response;
        }
    }

    nlohmann::json UserController::UpdateUserName(const nlohmann::json &request_body){
        nlohmann::json response;
        try {
            service_.UpdateUserName(request_body.at(BDNames::user_table_id),
                                    request_body.at(BDNames::user_table_user_name));
            response["status"] = ServerMessage::ResponseCode::OK;
            return response;
        }catch(const std::exception &e){
            ResponseError(response, e.what());
            return response;
        }
    }

    nlohmann::json UserController::UpdateUserPassword(const nlohmann::json &request_body){
        nlohmann::json response;
        try {
            service_.UpdateUserPassword(request_body.at(BDNames::user_table_id),
                                        request_body.at(BDNames::user_table_password));
            response["status"] = ServerMessage::ResponseCode::OK;
            return response;
        }catch(const std::exception &e){
            ResponseError(response, e.what());
            return response;
        }
    }

    nlohmann::json UserController::UpdateUserBalance(const nlohmann::json &request_body){
        nlohmann::json response;
        try {
            service_.UpdateUserBalance(request_body.at(BDNames::user_table_id),
                                       request_body.at(BDNames::user_table_balance));
            response["status"] = ServerMessage::ResponseCode::OK;
            return response;
        }catch(const std::exception &e){
            ResponseError(response, e.what());
            return response;
        }
    }

    nlohmann::json UserController::DeleteUser(const nlohmann::json &request_body){
        nlohmann::json response;
        try {
            service_.DeleteUser(request_body.at(BDNames::user_table_id));
            response["status"] = ServerMessage::ResponseCode::OK;
            return response;
        }catch(const std::exception &e){
            ResponseError(response, e.what());
            return response;
        }
    }

    void UserController::ResponseError(nlohmann::json &response, const char *exception) {
        response["status"] = ServerMessage::response_code.find(exception) != ServerMessage::response_code.end()
                             ? ServerMessage::response_code.at(exception)
                             : ServerMessage::ResponseCode::BAD_REQUEST;
        response["message"] = exception;
    }
}