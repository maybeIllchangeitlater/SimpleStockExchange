#ifndef SIMPLESTOCKEXCHANGE_CLIENT_HPP
#define SIMPLESTOCKEXCHANGE_CLIENT_HPP

#include <memory>
#include <iostream>
#include "../Connection/Connection.hpp"
#include "../Controller/ClientController.hpp"
#include "../Utility/ThreadSafeQ.hpp"
#include "../Utility/ExtraJSONKeys.hpp"
#include <boost/make_unique.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

namespace s21 {
    class Client {
        using connection_ptr = boost::shared_ptr<Connection>;
        using tcp = boost::asio::ip::tcp;
    public:
        Client() = default;

        bool Connect(const std::string &host, short port);
        void Disconnect();
        bool Connected();
        void Send(const std::string &message);

        ThreadSafeQ<std::pair<connection_ptr, std::string>> & Inbox(){
            return from_server_;
        }

        void Register(const std::string &username, const std::string &password,
                      const std::string &balance_usd, const std::string &balance_rub){
            Send(ClientController::Register(username, password, balance_usd, balance_rub));
        }

        void LogIn(const std::string &username, const std::string& password){
            Send(ClientController::Login(username, password));
        }

        void CreateSellBid(const std::string &quantity, const std::string &rate){
            Send(ClientController::CreateBid(GetUserId(), quantity, rate, BidService::SELLING));
        }

        void CreateBuyBid(const std::string &quantity, const std::string &rate){
            Send(ClientController::CreateBid(GetUserId(), quantity, rate, BidService::BUYING));
        }


        void GetMySellBids(){
            Send(ClientController::GetMySellBids(GetUserId()));
        }

        void GetMyBuyBids(){
            Send(ClientController::GetMyBuyBids(GetUserId()));
        }

        void GetMySellTransactions(){
            Send(ClientController::GetMySellTransactions(GetUserId()));
        }

        void GetMyBuyTransactions(){
            Send(ClientController::GetMyBuyTransactions(GetUserId()));
        }

        void CancelBid(const std::string &bid_id){
            Send(ClientController::CancelBid(bid_id, user_id_));
        }

        void ChangeName(){
            std::string name;
            std::cout << "Please enter your new username\n";
            std::cin >> name;
            Send(ClientController::ChangeUserName(GetUserId(), name));
        }

        void ChangePassword(){
            std::string password;
            std::cout << "Please enter your new password\n";
            std::cin >> password;
            Send(ClientController::ChangePassword(GetUserId(), password));
        }

        void DeleteMe(){
            Send(ClientController::DeleteMe(GetUserId()));
            while (from_server_.Empty()) {}
            Disconnect();
        }

        void UpdateBidRate(const std::string &bid_id, const std::string &new_rate){
            Send(ClientController::UpdateBidRate(bid_id, new_rate));
        }

        void UpdateBidQuantity(const std::string &bid_id, const std::string &new_quantity){
            Send(ClientController::UpdateBidQuantity(bid_id, new_quantity));
        }

        void CheckBalance(){
            Send(ClientController::GetMyBalance(user_id_));
        }

        void WaitForResponse(){
            while (from_server_.Empty()) {}
        }
        bool CheckStatus(){
            return from_server_.Front().second.find("OK") != std::string::npos;
        }

        void Authorize(){
            auto msg = from_server_.PopFront().second;
            size_t json_start = msg.find('{');
            size_t json_end = msg.rfind('}');
            std::string json_str = msg.substr(json_start, json_end - json_start + 1);
            nlohmann::json json = nlohmann::json::parse(json_str);
            user_id_ = json["id"];
        }

        std::string CleanServerResponse(){
            auto msg = from_server_.PopFront().second;
            std::cout << msg << "\n";
            bool error = msg.find("message") != std::string::npos;
                size_t json_start = error
                        ? msg.find_last_of('{')
                        : msg.find('{');

                size_t json_end = error
                        ? msg.find('}')
                        : msg.rfind('}');
                return msg.substr(json_start +1 , json_end - json_start - 1);
        }

        bool CheckIfTransactionsWereMade(){
            return from_server_.Front().second.find(s21::ExtraJSONKeys::buy_transaction) != std::string::npos
                    ||from_server_.Front().second.find(s21::ExtraJSONKeys::sell_transaction) != std::string::npos;
        }

        std::string CleanBidCreatedResponse(){
            auto msg = CleanServerResponse();
            msg.erase(std::remove_if(msg.begin(), msg.end(), [](char c) { return c == '\"' || c == ':'; }), msg.end());

            std::string second_part;
            std::string first_part;
            first_part = msg.substr(msg.find(',') + 1);
            second_part = msg.substr(msg.find('{') + 1, msg.find(','));

            return first_part + second_part + " RUB per 1 USD";
        }

        std::string CleanBidUpdateRespons(std::string &server_response){
            server_response.erase(std::remove(server_response.begin(), server_response.end(), '"'), server_response.end());

            std::string msg_bid_type = server_response.find(s21::BDNames::missing_buyer) != std::string::npos
                    ? " sell "
                    : " buy ";

            auto msg_bid = server_response.substr(server_response.find(s21::BDNames::bid_id_for_join) + 7,
                                      server_response.find(',') - server_response.find(s21::BDNames::bid_id_for_join) - 7);

            auto quant_start_pos = server_response.find(s21::BDNames::bid_table_quantity);
            auto msg_quant = server_response.substr(quant_start_pos, server_response.find(',', quant_start_pos) - quant_start_pos);

            auto rate_start_pos = server_response.find(s21::BDNames::bid_table_rate);
            auto msg_rate = server_response.substr(rate_start_pos, server_response.find(',', rate_start_pos) - rate_start_pos);
            return "Updated" + msg_bid_type +  "bid :\n" + msg_bid + "\nto " + msg_quant + " and " + msg_rate;
        }




    private:
        const std::string &GetUserId(){
            return user_id_;
        }
        std::unique_ptr<boost::asio::io_context> context_;
        boost::thread thread_context_;
        std::unique_ptr<Connection> connection_;
        ThreadSafeQ<std::pair<connection_ptr, std::string>> from_server_;
        std::string user_id_;
    };
}

#endif //SIMPLESTOCKEXCHANGE_CLIENT_HPP
