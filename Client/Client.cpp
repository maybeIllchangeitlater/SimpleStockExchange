#include "Client.hpp"

namespace s21{
    bool Client::Connect(const std::string &host, short port) {
        try{
            context_.restart();
            tcp::resolver resolver(context_);
            tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));
            connection_ = boost::make_unique<Connection>(Connection::Owner::CLIENT, context_,
                                                              tcp::socket(context_), from_server_);
            connection_->ConnectToServer(endpoints);
            std::cout << "Client : Connected to remote server\n";
                thread_context_ = boost::thread([this](){ context_.run(); });

        }catch(const std::exception &e){
            std::cout << "Client : Connection failed with exception : " << e.what() << std::endl;
            return false;
        }
        return true;
    }

    void Client::Disconnect() {
        if(Connected()){
            Send(ClientController::Logout());
//            boost::this_thread::sleep_for(boost::chrono::milliseconds(70));
            context_.stop();
            if(thread_context_.joinable())
                thread_context_.join();
            connection_->Disconnect();
            connection_.reset();
            from_server_.Clear();
            dcd = true;
        }
        std::cout << "dcd\n";
    }

    bool Client::Connected() {
        return connection_ && connection_->Connected();
    }

    void Client::Send(const std::string &message) {
        if(Connected()){
            std::cout << "Sending out " << message << std::endl;
            connection_->Send(message);
        }
    }
}
