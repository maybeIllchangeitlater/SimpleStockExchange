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

        const std::string &GetUserId(){
            if(user_id_.empty()) {
                Send(ClientController::GetMyId());
                while (from_server_.Empty()) {}
                user_id_ = from_server_.PopFront().second;
            }
            return user_id_;
        }

        std:: string GetIdFromHTTP(){
            auto msg = from_server_.PopFront().second;
            size_t jsonStart = msg.find('{');
            size_t jsonEnd = msg.rfind('}');
            //  // Find the start and end positions of the JSON part
            //    size_t jsonStart = input.find('{');
            //    size_t jsonEnd = input.rfind('}');
            //
            //    if (jsonStart != std::string::npos && jsonEnd != std::string::npos && jsonStart < jsonEnd) {
            //        // Extract the JSON part
            //        std::string jsonStr = input.substr(jsonStart, jsonEnd - jsonStart + 1);
            //
            //        try {
            //            // Parse the JSON string
            //            json j = json::parse(jsonStr);
            //
            //            // Extract the ID as a string
            //            std::string id = j["id"];
            //
            //            // Output the result
            //            std::cout << "Extracted ID: " << id << std::endl;
            //        } catch (const std::exception& e) {
            //            std::cerr << "Error parsing JSON: " << e.what() << std::endl;
            //        }
            //    } else {
            //        std::cerr << "No valid JSON found in the input string." << std::endl;
            //    }
        }


    private:
        boost::asio::io_context context_;
        boost::thread thread_context_;
        std::unique_ptr<Connection> connection_;
        ThreadSafeQ<std::pair<connection_ptr, std::string>> from_server_;
        std::string user_id_;
    };
}

#endif //SIMPLESTOCKEXCHANGE_CLIENT_HPP
