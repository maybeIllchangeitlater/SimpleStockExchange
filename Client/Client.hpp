#ifndef SIMPLESTOCKEXCHANGE_CLIENT_CLIENT_HPP
#define SIMPLESTOCKEXCHANGE_CLIENT_CLIENT_HPP

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <string>
#include <memory> //for unique ptr
#include <boost/make_unique.hpp>
#include "../Utility/ThreadSafeQ.hpp"
#include <iostream>
#include "../Connection/Connection.hpp"
#include "../Controller/ClientController.hpp"

namespace s21 {
    class Client{
        using tcp = boost::asio::ip::tcp;
        using connection_ptr = boost::shared_ptr<Connection>;

public:

        Client() = default;

        ~Client(){
            Disconnect();
        }

        bool Connect(const std::string &host, const short port);

        void Disconnect();

        bool Connected(){
            return connection_ && connection_->Connected();
        }

        void Send(const std::string &message){
            if(Connected()){
                connection_->Send(message);
            }
        }

        ThreadSafeQ<std::pair<connection_ptr, std::string>> &Incoming(){
            return connection_->Incoming();
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
            std::string balance;
            std::cout << "Please Enter Username, Password and Balance\n";
            std::cin >> username >> password >> balance;
            Send(ClientController::Login(username, password, balance));
        }

        void CreateSellBid(){
            std::string quantity;
            std::string rate;
            std::cout << "Please Enter Quantity and Rate\n";
            Send(ClientController::CreateBid(quantity, rate, BidService::SELLING));
        }

        void CreateBuyBid(){
            std::string quantity;
            std::string rate;
            std::cout << "Please Enter Quantity and Rate\n";
            Send(ClientController::CreateBid(quantity, rate, BidService::BUYING));
        }


        void GetMySellBids(){
            Send(ClientController::GetMySellBids(connection_->GetUserId()));
        }

        void GetMyBuyBids(){
            Send(ClientController::GetMyBuyBids(connection_->GetUserId()));
        }

        void GetMySellTransactions(){
            Send(ClientController::GetMySellTransactions(connection_->GetUserId()));
        }

        void GetMyBuyTransactions(){
            Send(ClientController::GetMyBuyTransactions(connection_->GetUserId()));
        }

        void CancelBid(){
            std::string bid_id; //validate that its own bid id
            std::cout << "Please enter bid_id\n";
            std::cin >> bid_id;
            Send(ClientController::CancelBid(bid_id));
        }


private:
    boost::asio::io_context context_;
    boost::thread thread_context_;
    Connection::connection_ptr connection_;
    ThreadSafeQ<std::pair<connection_ptr, std::string>> message_in_q_;
    };
}

#endif //SIMPLESTOCKEXCHANGE_CLIENT_CLIENT_HPP
