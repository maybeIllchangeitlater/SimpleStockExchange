#ifndef SIMPLESTOCKEXCHANGE_SERVER_HPP
#define SIMPLESTOCKEXCHANGE_SERVER_HPP
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "../Utility/ThreadSafeQ.hpp"
#include "../Connection/Connection.hpp"
#include "../Controller/ControllerMapping.hpp"
#include "../Controller/UserController.hpp"
#include "../Controller/BidController.hpp"
#include "../Controller/TransactionController.hpp"
#include <deque>

namespace s21 {
    class Server {
        using connection_ptr = boost::shared_ptr<Connection>;
        using tcp = boost::asio::ip::tcp;
        using error_code = boost::system::error_code;
    public:
        Server(short port, UserController &uc, BidController &bc, TransactionController &tc)
        : acceptor_(context_, tcp::endpoint(tcp::v4(), port)),
        user_controller_(uc), bid_controller_(bc), transaction_controller_(tc){}
        ~Server() { Stop(); }

        bool Start();
        void Stop();

        void WaitForClient();
        void MessageClient(connection_ptr, const std::string &message);
        void Update();

    private:
        bool LoginRegisterAttempt(const std::string &message){
            return message.find("Login: ") != std::string::npos || message.find("Register: ") != std::string::npos;
        }
        void OnConnect(connection_ptr);
        void OnDisconnect(connection_ptr);
        void OnMessage(connection_ptr, const std::string &message);
        // Thread Safe Queue for incoming message packets
        ThreadSafeQ<std::pair<connection_ptr, std::string>> in_;
        // Container of active validated connections
        std::deque<connection_ptr> connections_;

        // Order of declaration is important - it is also the order of initialisation
        boost::asio::io_context context_;
        boost::thread thread_context_;

        // These things need an asio context
        tcp::acceptor acceptor_; // Handles new incoming connection attempts...
        UserController &user_controller_;
        BidController &bid_controller_;
        TransactionController &transaction_controller_;
    };
}

#endif //SIMPLESTOCKEXCHANGE_SERVER_HPP
