#ifndef SIMPLESTOCKEXCHANGE_SERVER_HPP
#define SIMPLESTOCKEXCHANGE_SERVER_HPP

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "../Utility/ServerMessage.hpp"
#include "../Utility/ThreadSafeQ.hpp"
#include <iostream> //debug remove
#include "../HTTP/Http.hpp"
#include "../Connection/Connection.hpp"
#include <boost/noncopyable.hpp>
#include "../Controller/UserController.hpp"
#include "../Controller/BidController.hpp"
#include "../Controller/TransactionController.hpp"
#include "../Controller/ControllerMapping.hpp"



namespace s21 {
    using tcp = boost::asio::ip::tcp;
    using error_code = boost::system::error_code;
    using connection_ptr = boost::shared_ptr<Connection>;
    class Server : public boost::noncopyable {

    public:

        Server(short port, UserController& uc, BidController& bc, TransactionController& tc)
        : context_(), acceptor_(context_, tcp::endpoint(tcp::v4(), port)),
        user_controller_(uc), bid_controller_(bc), transaction_controller_(tc){}

        ~Server(){ Stop(); }

        bool Start();

        void Stop();

        void WaitForClient();

        void MessageClient(connection_ptr client, const std::string &message);

        void Update(size_t max_messages = -1, bool wait = false);

    private:

        bool OnClientConnect(connection_ptr client);

        void OnClientDisconnect(connection_ptr client){
            std::cout << "Removing client " << client->GetUserId();
        }

        void OnMessage(connection_ptr client, const std::string &message);

        bool LoginAttempt(const std::string &message){
            return message.find("Login: ") != std::string::npos;
        }

        boost::asio::io_context context_;
        boost::thread thread_context_;
        tcp::acceptor acceptor_;
        ThreadSafeQ<std::pair<connection_ptr, std::string>> message_in_q_;
        ThreadSafeQ<connection_ptr> connections_q_;
        UserController& user_controller_;
        BidController& bid_controller_;
        TransactionController& transaction_controller_;

    };
}

#endif //SIMPLESTOCKEXCHANGE_SERVER_HPP
