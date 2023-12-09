#include "TransactionController.hpp"

namespace s21{
    nlohmann::json TransactionController::MakeTransaction(const nlohmann::json &request_body){
        nlohmann::json response;
        try {
            service_.MakeTransaction(request_body.at(BDNames::transaction_table_seller_id),
                                     request_body.at(BDNames::transaction_table_buyer_id),
                                     request_body.at(BDNames::transaction_table_rate),
                                     request_body.at(BDNames::transaction_table_quantity));
            response["status"] = ServerMessage::ResponseCode::OK;
            return response;
        }catch(const std::exception &e){
            response["status"] = ServerMessage::response_code.find(e.what()) != ServerMessage::response_code.end()
                                 ? ServerMessage::response_code.at(e.what())
                                 : ServerMessage::ResponseCode::BAD_REQUEST;
            return response;
        }
    }
        //need to notify
    nlohmann::json TransactionController::GetTransaction(const nlohmann::json &request_body){
            nlohmann::json response;
            try {
                response = service_.ReadTransaction(request_body.at(BDNames::transaction_table_id));
                response["status"] = ServerMessage::ResponseCode::OK;
                return response;
            }catch(const std::exception &e){
                response["status"] = ServerMessage::response_code.find(e.what()) != ServerMessage::response_code.end()
                                     ? ServerMessage::response_code.at(e.what())
                                     : ServerMessage::ResponseCode::BAD_REQUEST;
                return response;
            }
    }

    nlohmann::json TransactionController::GetUserSellTransactions(const nlohmann::json &request_body) {
        nlohmann::json response;
        try {
            response = service_.ReadAllUserSellTransactions(request_body.at(BDNames::transaction_table_id));
            response["status"] = ServerMessage::ResponseCode::OK;
            return response;
        }catch(const std::exception &e){
            response["status"] = ServerMessage::response_code.find(e.what()) != ServerMessage::response_code.end()
                                 ? ServerMessage::response_code.at(e.what())
                                 : ServerMessage::ResponseCode::BAD_REQUEST;
            return response;
        }
    }

    nlohmann::json TransactionController::GetUserBuyTransactions(const nlohmann::json &request_body) {
        nlohmann::json response;
        try {
            response = service_.ReadAllUserBuyTransactions(request_body.at(BDNames::transaction_table_id));
            response["status"] = ServerMessage::ResponseCode::OK;
            return response;
        }catch(const std::exception &e){
            response["status"] = ServerMessage::response_code.find(e.what()) != ServerMessage::response_code.end()
                                 ? ServerMessage::response_code.at(e.what())
                                 : ServerMessage::ResponseCode::BAD_REQUEST;
            return response;
        }
    }

}