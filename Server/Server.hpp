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
#include "../Utility/ExtraJSONKeys.hpp"
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
        void Update();
        void RemoveClient(connection_ptr client);

    private:
        bool LoginRegisterAttempt(const std::string &message){
            return message.find("Authenticate") != std::string::npos || message.find("Register") != std::string::npos;
        }
        void OnConnect(connection_ptr);
        void OnDisconnect(connection_ptr);
        void OnMessage(connection_ptr, const std::string &message);
        ThreadSafeQ<std::pair<connection_ptr, std::string>> in_;
        std::vector<connection_ptr> connections_;

        boost::asio::io_context context_;
        boost::thread thread_context_;
        tcp::acceptor acceptor_;

        UserController &user_controller_;
        BidController &bid_controller_;
        TransactionController &transaction_controller_;
    };
}

#endif //SIMPLESTOCKEXCHANGE_SERVER_HPP
