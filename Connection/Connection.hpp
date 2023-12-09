#ifndef SIMPLESTOCKEXCHANGE_CONNECTION_HPP
#define SIMPLESTOCKEXCHANGE_CONNECTION_HPP

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include "../Utility/ThreadSafeQ.hpp"
#include <iostream>
#include <vector>
#include "../HTTP/Http.hpp"
#include "../Controller/UserController.hpp"
#include "../Controller/BidController.hpp"
#include "../Controller/TransactionController.hpp"
#include "../Controller/ControllerMapping.hpp"

namespace s21 {
    class Connection : public boost::enable_shared_from_this<Connection> {
    public:
        using connection_ptr = boost::shared_ptr<Connection>;
        using tcp = boost::asio::ip::tcp;
        using error_code = boost::system::error_code;
        enum Owner{
            SERVER,
            CLIENT,
        };

        Connection(Owner parent, boost::asio::io_context &context, tcp::socket &&socket,
                   ThreadSafeQ<std::pair<connection_ptr, std::string>> &messageq_in)
                   :owner_(parent), context_(context), socket_((std::move(socket))), message_in_q_(messageq_in){
        }

        const std::string &GetUserId(){
            return user_id;
        }

        void ConnectToClient(){
            if(owner_ == SERVER && Connected()){
                ReadHeader();
            }
        }

        void ConnectToServer(const tcp::resolver::results_type& endpoints){
            if(owner_ == CLIENT){
                boost::asio::async_connect(socket_, endpoints, [this](error_code ec, tcp::endpoint endpoint){
                    if(!ec){
                        ReadHeader();
                    }
                });
            }
        }

        void Disconnect(){
            if(Connected()){
                boost::asio::post(context_, [this](){ socket_.close(); });
            }
        }

        bool Connected() const{
            return socket_.is_open();
        }

        bool Authorized() const{
            return authorized_;
        }

        void Authorize(const std::string &userid){
            user_id = userid;
            authorized_ = true;
        }

        void Listen(){
            ReadHeader();
        }

        void Send(const std::string & message);


    private:
        void Write();

        void ReadHeader();

        std::string ReadBody(size_t body_length);

        void AddToInQueue(){
            message_in_q_.EmplaceBack(std::make_pair(shared_from_this(), unfinished_message_));
            ReadHeader();
        }

        size_t FindContentLength(const std::string &headers){
            std::string cl("Context-Length: ");
            size_t pos = headers.find(cl);
            return pos != std::string::npos ? std::stoul(headers.substr(pos + cl.length())) : 0;
        }

        tcp::socket socket_;
        boost::asio::io_context &context_;
        ThreadSafeQ<std::pair<connection_ptr, std::string>> message_out_q_;
        ThreadSafeQ<std::pair<connection_ptr, std::string>> &message_in_q_;
        std::string input_buffer_;
        std::string unfinished_message_;
        Owner owner_;
        std::string user_id;
        bool authorized_ = false;
    };
}

#endif //SIMPLESTOCKEXCHANGE_CONNECTION_HPP
