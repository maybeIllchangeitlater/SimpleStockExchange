#include "Client.hpp"

namespace s21{
    bool Client::Connect(const std::string &host, const short port) {
        try{
            tcp::resolver resolver(context_);
            tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));
            connection_ = boost::make_unique<Connection>(Connection::Owner::CLIENT, context_,
                                                         std::move(tcp::socket(context_)), message_in_q_);
            connection_->ConnectToServer(endpoints);
            boost::asio::io_context::work work(context_);
            thread_context_ = boost::thread([this](){ context_.run(); });
            std::cout << "Im in\n" << std::endl;
            return true;
        }catch(const std::exception &e){
            std::cout << e.what();
            return false;
        }
    }

    void Client::Disconnect() {
        if(Connected()){
            connection_->Disconnect();
        }
        context_.stop();
        if(thread_context_.joinable()){
            thread_context_.join();
        }
        connection_.reset();
    }
}