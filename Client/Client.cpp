#include "Client.hpp"

namespace s21{
    bool Client::Connect(const std::string &host, short port) {
        try{
            context_ = boost::make_unique<boost::asio::io_context>();
            tcp::resolver resolver(*context_);
            tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));
            connection_ = boost::make_unique<Connection>(Connection::Owner::CLIENT, *context_,
                                                              tcp::socket(*context_), from_server_);
            connection_->ConnectToServer(endpoints);
                thread_context_ = boost::thread([this](){ context_->run(); });

        }catch(const std::exception &e){
            return false;
        }
        return true;
    }

    void Client::Disconnect() {
        if(Connected()){
            Send(ClientController::Logout());
            try{
                WaitForResponse();
                from_server_.PopFront();
            }catch(...){}
            CutConnection();
        }
    }

    bool Client::Connected() {
        return connection_ && connection_->Connected();
    }

    void Client::Send(const std::string &message) {
        if(Connected()){
            connection_->Send(message);
        }
    }
}
