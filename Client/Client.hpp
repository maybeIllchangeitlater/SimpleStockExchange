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
            connection_ && connection_->Connected();
        }

        void Send(const std::string &message){
            if(Connected()){
                connection_->Send(message);
            }
        }

        ThreadSafeQ<std::pair<connection_ptr, std::string>> &Incoming(){
            return message_in_q_;
        }

private:
    boost::asio::io_context context_;
    boost::thread thread_context_;
    std::unique_ptr<Connection> connection_;
    ThreadSafeQ<std::pair<connection_ptr, std::string>> message_in_q_;
    };
}

#endif //SIMPLESTOCKEXCHANGE_CLIENT_CLIENT_HPP
