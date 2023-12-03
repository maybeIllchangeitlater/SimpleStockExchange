#ifndef SIMPLESTOCKEXCHANGE_SERVER_HPP_
#define SIMPLESTOCKEXCHANGE_SERVER_HPP_

#include "../Controllers/Handlers.hpp"
#include <iostream>

namespace s21 {
    class Server {
    public:
        using client_ptr = boost::shared_ptr<Session>;

        Server(boost::asio::io_context& context)
        : context_(context), handle_(context, [&](client_ptr client){
            AddClient(client);
        }) {
            auto client = Session::MakeShared(context, [&](client_ptr client){
                RemoveClient(client);
            });
            handle_.GetAcceptor().async_accept(client->GetSocket(),
                                               boost::bind(&Handlers::HandleAccept, &handle_, client, _1, _2));
            Listen();
        }

    private:
        void Listen(){
            context_.run();
        }

        void AddClient(client_ptr client){
            boost::recursive_mutex::scoped_lock lock(clients_lock_);
            clients_.emplace_back(client);
        }

        void RemoveClient(client_ptr client){
            boost::recursive_mutex::scoped_lock lock(clients_lock_);
            vacant_spots_.emplace_back(std::distance(clients_.begin(),
                                                     std::find(clients_.begin(), clients_.end(), client)));
        }

        Handlers handle_;
        boost::recursive_mutex clients_lock_;
        std::vector<boost::shared_ptr<Session>> clients_;
        std::vector<size_t> vacant_spots_;

        boost::asio::io_context &context_;

        ///conext is passed here from main, acceptor is created and stored here




    };
} //s21

#endif //SIMPLESTOCKEXCHANGE_SERVER_HPP_
