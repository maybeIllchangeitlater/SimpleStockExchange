#include "Server.hpp"
namespace s21{
    bool Server::Start(){
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

    void Server::Stop() {
        context_.stop();
        if(thread_context_.joinable())
            thread_context_.join();
        std::cout << "Server stopped\n";
    }

    void Server::WaitForClient() {
        acceptor_.async_accept([this](error_code ec, tcp::socket socket) {
            if(!ec){
                std::cout << "New Server connection: " << socket.remote_endpoint() << std::endl;
                connection_ptr new_connection =
                        boost::make_shared<Connection>(Connection::Owner::SERVER,
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

    void Server::MessageClient(connection_ptr client, const std::string &message) {
        std::cout << "trying to message client\n";
        if(client && client->Connected()){
            client->Send(message);
        }else{
            OnClientDisconnect(client);
            client.reset();
            connections_q_.Erase(client);
        }
    }

    void Server::Update(size_t max_messages, bool wait) {
        std::cout << "here in update\n";
        if(wait)
            message_in_q_.Wait();
        size_t message_count = 0;
        while (message_count < max_messages && !message_in_q_.Empty())
        {
            auto msg = message_in_q_.PopFront();
            OnMessage(msg.first, msg.second); //msg must contain sender
            ++message_count;
        }
    }

    bool Server::OnClientConnect(connection_ptr client) {
        std::cout << "here in client connected\n";
        if(!client->Connected())
            return false;
        Response response;
        response.body = ServerMessage::server_message.at(ServerMessage::WELCOME);
//        std::cout << "welcome message is : " << response.body.dump();
        response.status = ServerMessage::response_code.at(ServerMessage::server_message.at(ServerMessage::WELCOME));
//        std::cout << "\nresponse code is : " << response.status;
        client->Send(response.ToString());
        return true;
    }

    void Server::OnMessage(s21::connection_ptr client, const std::string &message) {
        std::cout << "here at readmsg start\n";
        Response response;
        nlohmann::json result;
        if(client->Authorized() || LoginRegisterAttempt(message)) {
            auto request = Request::Parse(message);
            if (request.path[0] == 'U') {
                result = ControllerMapping::method_mapping_user.at(request.path.substr(1))(user_controller_,
                                                                                           request.body);
            } else if (request.path[0] == 'B') {
                result = ControllerMapping::method_mapping_bid.at(request.path.substr(1))(bid_controller_,
                                                                                          request.body);
            } else if (request.path[0] == 'T') {
                result = ControllerMapping::method_mapping_transaction.at(request.path.substr(1))(
                        transaction_controller_, request.body);
            }
            response.status = result["status"];
            if(LoginRegisterAttempt(message) && response.status == ServerMessage::ResponseCode::OK){
                client->Authorize(result[BDNames::user_table_id]);
            }
            result.erase("status");
            response.body = result;
        }else{
            std::cout << "here at readmsg error\n";
            response.status = ServerMessage::ResponseCode::BAD_REQUEST;
            response.body = ServerMessage::server_message.at(ServerMessage::NOT_LOGGED_IN);
        }
        client->Send(response.ToString());
    }
}