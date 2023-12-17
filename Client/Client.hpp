#ifndef SIMPLESTOCKEXCHANGE_CLIENT_HPP
#define SIMPLESTOCKEXCHANGE_CLIENT_HPP

#include <memory>
#include <iostream>
#include "../Connection/Connection.hpp"
#include "../Controller/ClientController.hpp"
#include "../Utility/ThreadSafeQ.hpp"
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

        void Register(const std::string &username, const std::string &password, const std::string &balance){
            Send(ClientController::Register(username, password, balance));
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

        void UpdateBidRate(){
            std::string bid_id;
            std::string new_rate;
            std::cout << "Please enter bid_id and new rate\n";
            std::cin >> bid_id >> new_rate;
            Send(ClientController::UpdateBidRate(bid_id, new_rate));
        }

        void UpdateBidQuantity(){
            std::string bid_id;
            std::string new_quantity;
            std::cout << "Please enter bid_id and new quantity\n";
            std::cin >> bid_id >> new_quantity;
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
