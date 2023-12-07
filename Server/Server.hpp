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

class Connection;

namespace s21 {
    using tcp = boost::asio::ip::tcp;
    using error_code = boost::system::error_code;
    using connection_ptr = boost::shared_ptr<Connection>;
    class Server {
    public:
        Server(short port) : acceptor_(boost::asio::io_context, tcp::endpoint(tcp::v4(), port)){}

        ~Server(){ Stop(); }

        bool Start(){
            try{
                WaitForClient();
                thread_context_ = boost::thread([this](){ context_.run(); });
            }catch(const std::exception &e){
                ///display exception on thread start
                std::cout << ServerMessage::SERVER_FAILED_TO_START << " with error: " << e.what();
                return false;
            }
            std::cout << "Server is running\n";
            return true;
        }

        void Stop(){
            context_.stop();
            if(thread_context_.joinable())
                thread_context_.join();
            std::cout << "Server stopped\n";
        }

        void WaitForClient(){
            acceptor_.async_accept([this](error_code ec, tcp::socket socket) {
                if(!ec){
                    std::cout << "New Server connection: " << socket.remote_endpoint() << std::endl;
                    connection_ptr new_connection =
                            boost::make_shared<Connection>(Connection::owner::server,
                                                           context_, std::move(socket), message_in_q_);
                    if(OnClientConnect(new_connection)){
                        connections_q_.EmplaceBack(std::move(new_connection));
                        connections_q_.Back()->ConnectToClient();
                        ///get id to cout conenction
                    }else{
                        std::cout << "Connection denied by server"; //send as actual message
                    }
                }else{
                    std::cout << ec.message() ;
                }
                WaitForClient();
            });
        }

        void MessageClient(connection_ptr client, const std::string &message){
            if(client && client->Connected()){
                client->Send(message);
            }else{
                OnClientDisconnect(client);
                client.reset();
                connections_q_.Erase(client);
            }
        }

        void Update(size_t max_messages = -1, bool wait = false){
            if(wait)
                message_in_q_.Wait();
            size_t message_count = 0;
            while (message_count < max_messages && !message_in_q_.Empty())
            {
                auto msg = message_in_q_.PopFront();
                OnMessage(msg); //msg must contain sender
                ++message_count;
            }

        }
    private:
        bool OnClientConnect(connection_ptr client){
            Response response;
            response.body = ServerMessage::server_message.at(ServerMessage::WELCOME);
            response.status = ServerMessage::response_code.at(ServerMessage::server_message.at(ServerMessage::WELCOME));
            client->Send(response);
            return true;
        }

        void OnClientDisconnect(connection_ptr client){
            std::cout << "Removing client " << client->GetId();
        }

        void OnMessage(connection_ptr client, const std::string &message){
            //impl
        }

        boost::asio::io_context context_;
        boost::thread thread_context_;
        tcp::acceptor acceptor_;
        ThreadSafeQ<std::string> message_in_q_;
        ThreadSafeQ<connection_ptr> connections_q_;
    };
}

#endif //SIMPLESTOCKEXCHANGE_SERVER_HPP
