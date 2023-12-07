#ifndef SIMPLESTOCKEXCHANGE_CLIENT_CLIENT_HPP
#define SIMPLESTOCKEXCHANGE_CLIENT_CLIENT_HPP

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
//#include <boost/enable_shared_from_this.hpp>
#include <boost/thread.hpp>
#include <string>
#include <memory> //for unique ptr
#include <boost/make_unique.hpp>
#include "../Utility/ThreadSafeQ.hpp"
#include <iostream>

namespace s21 {
    class Connection;
    class Client{
        using tcp = boost::asio::ip::tcp;
public:
        Client() = default;
        ~Client(){
            Disconnect();
        }
        bool Connect(const std::string &host, const short port){
            try{
                tcp::resolver resolver(context_);
                tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));
                connection_ = boost::make_unique<Connection>(Connection::owner::client, context_,
                                                             std::move(tcp::socket(context_)), message_in_q_);
                connection_->ConnectToServer(endpoints);
                thread_context_ = boost::thread([this](){ context_.run(); });
                return true;
            }catch(const std::exception &e){
                std::cout << e.what();
                return false;
            }
        }
        void Disconnect(){
            if(Connected()){
                connection_->Disconnect();
            }
            context_.stop();
            if(thread_context_.joinable()){
                thread_context_.join();
            }
            connection_.release();
        }

        bool Connected(){
            connection_ ? connection_->Connected() : false;
        }

        void Send(const std::string &message){
            if(Connected()){
                connection_->Send(message);
            }
        }

        ThreadSafeQ<std::string> Incoming(){
            return message_in_q_;
        }

private:
    const std::string user_id_;
        boost::asio::io_context context_;
    boost::thread thread_context_;
    std::unique_ptr<Connection> connection_;
    ThreadSafeQ<std::string> message_in_q_;
    };
}

#endif //SIMPLESTOCKEXCHANGE_CLIENT_CLIENT_HPP
