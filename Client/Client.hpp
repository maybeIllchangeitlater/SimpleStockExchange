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

        void Register(){
            std::string username;
            std::string password;
            std::string balance;
            std::cout << "Please Enter Username, Password and Balance\n";
            std::cin >> username >> password >> balance;
            Send(ClientController::Register(username, password, balance));
        }

        void LogIn(){
            std::string username;
            std::string password;
            std::cout << "Please Enter Username and Password\n";
            std::cin >> username >> password;
            Send(ClientController::Login(username, password));
        }

        void CreateSellBid(){
            std::string quantity;
            std::string rate;
            std::cout << "Please Enter Quantity and Rate\n";
            std::cin >> quantity >> rate;
            Send(ClientController::CreateBid(GetUserId(), quantity, rate, BidService::SELLING));
        }

        void CreateBuyBid(){
            std::string quantity;
            std::string rate;
            std::cout << "Please Enter Quantity and Rate\n";
            std::cin >> quantity >> rate;
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

        void CancelBid(){
            std::string bid_id; //validate that its own bid id
            std::cout << "Please enter bid_id\n";
            std::cin >> bid_id;
            Send(ClientController::CancelBid(bid_id));
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



    private:
        const std::string &GetUserId(){
            if(user_id_.empty()) {
                Send(ClientController::GetMyId());
                while (from_server_.Empty()) {}
                user_id_ = GetIdFromHTTP();
            }
            return user_id_;
        }

        std:: string GetIdFromHTTP(){
            auto msg = from_server_.PopFront().second;
            size_t json_start = msg.find('{');
            size_t json_end = msg.rfind('}');
            std::string json_str = msg.substr(json_start, json_end - json_start + 1);
            nlohmann::json json = nlohmann::json::parse(json_str);
            std::string id = json["id"];
            return id;
        }
        boost::asio::io_context context_;
        boost::thread thread_context_;
        std::unique_ptr<Connection> connection_;
        ThreadSafeQ<std::pair<connection_ptr, std::string>> from_server_;
        std::string user_id_;
    };
}

#endif //SIMPLESTOCKEXCHANGE_CLIENT_HPP
