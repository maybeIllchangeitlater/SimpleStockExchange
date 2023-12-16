#include "Server.hpp"

namespace s21{

    bool Server::Start() {
        try{
            WaitForClient();
            thread_context_ = boost::thread([this](){ context_.run(); });
        }catch(const std::exception &e){
            std::cout << "Server failed to start\n Error : " << e.what();
            return false;
        }
        std::cout << "Server is running\n";
        return true;
    }

    void Server::Stop() {
        context_.stop();
        if(thread_context_.joinable()){
            thread_context_.join();
        }
        std::cout << "Server has stopped\n";
    }

    void Server::WaitForClient() {
        acceptor_.async_accept([this](error_code ec, tcp::socket socket){
            if(!ec) {
                std::cout << "Server: New Connection: " << socket.remote_endpoint() << "\n";
                connection_ptr new_connection = boost::make_shared<Connection>(Connection::Owner::SERVER, context_,
                                                                               std::move(socket), in_);
                OnConnect(new_connection);
                connections_.emplace_back(std::move(new_connection));
                connections_.back()->ConnectToClient();
            }else{
                std::cout << "Server: New Connection: error: " << ec.message() << "\n";
            }
            WaitForClient();
        });
    }

    void Server::RemoveClient(connection_ptr client){
        OnDisconnect(client);
        client.reset();
        connections_.erase(std::remove(connections_.begin(), connections_.end(), client), connections_.end());
    }

    void Server::Update() {
        while(!in_.Empty()){
            auto [client, message] = in_.PopFront();
            OnMessage(client, message);
        } ///Add Logic For Sending Transaction notifications?
    }

    void Server::OnDisconnect(connection_ptr) {
        std::cout << "Removing client \n";

    }

    void Server::OnConnect(connection_ptr client) {
        std::cout << "Server:: in client connected\n";
        Response response;
        response.body = ServerMessage::server_message.at(ServerMessage::WELCOME);
        response.status = ServerMessage::response_code.at(ServerMessage::server_message.at(ServerMessage::WELCOME));
        client->Send(response.ToString());
    }

    void Server::OnMessage(s21::Server::connection_ptr client, const std::string &message) {
        std::cout << "here at onmsg start\nMessage is:\n" << message << "\n";
        Response response;
        nlohmann::json result;
        if(client->Authorized() || (LoginRegisterAttempt(message) && !client->Authorized())) {
            auto request = Request::Parse(message);
            std::cout << "parse successful\n";
            if (request.path[0] == 'U') {
                std::cout << "parsing user request\n" << request.path.substr(1) << "\n";
                result = (ControllerMapping::method_mapping_user.at(request.path.substr(1)))(user_controller_,
                                                                                           request.body);
            } else if (request.path[0] == 'B') {
                std::cout << "parsing bid request\n" << request.path.substr(1) << "\nbody is :\n"
                << request.body.dump() << "\n";
                result = ControllerMapping::method_mapping_bid.at(request.path.substr(1))(bid_controller_,
                                                                                              request.body);
            } else if (request.path[0] == 'T') {
                result = ControllerMapping::method_mapping_transaction.at(request.path.substr(1))(
                        transaction_controller_, request.body);
            }
            else if (request.path[0] == 'D'){
                response.status = ServerMessage::ResponseCode::OK;
                response.body = ServerMessage::server_message.at(ServerMessage::WELCOME);
                client->Send(response.ToString());
                RemoveClient(client);
                return;
            }
            if(result.is_array()) {
                response.status = result.back()["status"];
            }else{
                response.status = result["status"];
            }
            if(LoginRegisterAttempt(message) && response.status == ServerMessage::ResponseCode::OK){
                client->Authorize(result[BDNames::user_table_id]);
            }
            if(result.is_array()) {
                result.erase(--result.end());
            }else{
                result.erase("status");
            }
            response.body = result;
        }else{
            std::cout << "here at readmsg error\n";
            response.status = ServerMessage::ResponseCode::BAD_REQUEST;
            response.body = ServerMessage::server_message.at(ServerMessage::NOT_LOGGED_IN);
        }
        client->Send(response.ToString());
    }


}