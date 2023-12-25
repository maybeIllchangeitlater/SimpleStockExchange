#include "TransactionController.hpp"

namespace s21{

    nlohmann::json TransactionController::GetUserSellTransactions(const nlohmann::json &request_body) {
        nlohmann::json response;
        try {
            response = service_.ReadAllUserSellTransactions(request_body.at(BDNames::transaction_table_id));
            nlohmann::json status;
            status[ExtraJSONKeys::status] = ServerMessage::OK;
            response.emplace_back(status);
            return response;
        }catch(const std::exception &e){
            ResponseError(response, e.what());
            return response;
        }
    }

    nlohmann::json TransactionController::GetUserBuyTransactions(const nlohmann::json &request_body) {
        nlohmann::json response;
        try {
            response = service_.ReadAllUserBuyTransactions(request_body.at(BDNames::transaction_table_id));
            nlohmann::json status;
            status[ExtraJSONKeys::status] = ServerMessage::OK;
            response.emplace_back(status);
            return response;
        }catch(const std::exception &e){
            ResponseError(response, e.what());
            return response;
        }
    }

    void TransactionController::ResponseError(nlohmann::json &response, const char *exception) {
        response[ExtraJSONKeys::status] = ServerMessage::response_code.find(exception) != ServerMessage::response_code.end()
                                          ? ServerMessage::response_code.at(exception)
                                          : ServerMessage::ResponseCode::BAD_REQUEST;
        response[ExtraJSONKeys::message] = exception;
    }

}