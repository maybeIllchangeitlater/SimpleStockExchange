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
    class Connection : boost::enable_shared_from_this<Connection> {
    public:
        using tcp = boost::asio::ip::tcp;
        using error_code = boost::system::error_code;
        enum Owner{
            SERVER,
            CLIENT,
        };

        Connection(Owner parent, boost::asio::io_context &context, tcp::socket &&socket,
                   ThreadSafeQ<std::string> &messageq_in, UserController & uc, BidController &bc,
                   TransactionController &tc)
                   :owner_(parent), context_(context), socket_((std::move(socket))), message_in_q_(messageq_in),
                   user_controller_(uc), bid_controller_(bc), transaction_controller_(tc){
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

        void Listen(){

        }

        void Send(const std::string & message){
            boost::asio::post(context_, [this, message](){
                bool write_message = message_out_q_.Empty();
                message_out_q_.EmplaceBack(message);
                if(write_message){
                   WriteHeader();
                }
            });
        }


    private:
        void WriteHeader(){
            boost::asio::async_write(socket_, boost::asio::buffer(&message_out_q_.Front(),
                                                                  sizeof(message_out_q_.Front())),
                                     [this](error_code ec, size_t length){
                if(!ec){
                    if(message_out_q_.Front().body.size() > 0){
                        WriteBody();
                    }else{
                        message_out_q_.PopFront();
                        if(!message_out_q_.Empty()){
                            WriteHeader();
                        }
                    }
                }else{
                    std::cout << "Header write failed, closing socket\n";
                    socket_.close();
                }
            });
        }

        void WriteBody(){
            boost::asio::async_write(socket_, boost::asio::buffer(message_out_q_.Front().data(),
                                                                  message_out_q_.Front().size()),
                                                                  [this](error_code ec, size_t length){
                if(!ec){
                    message_out_q_.PopFront();
                    if(!message_out_q_.Empty()){
                        WriteHeader();
                    }
                }else{
                    std::cout << "Body write failed, closing socket\n";
                    socket_.close();
                }
            });
        }

        void ReadHeader(){
            boost::asio::async_read_until(socket_, boost::asio::dynamic_buffer(input_buffer_), "\r\n\r\n",
                                    [this](error_code ec, size_t length){
                if(!ec){
                    std::string headers(input_buffer_.data(), input_buffer_.data() + length);
                    auto req = Request::ParseHeader(headers);
                    if(req.content_length)
                        ReadBody(req);
                    PassToController(req);
                }else{
                    std::cout << "Header Read failed, closing socket\n";
                    socket_.close();
                }
            });
        }

        void ReadBody(Request &request){
            input_buffer_.resize(request.content_length);
            boost::asio::async_read(socket_, boost::asio::buffer(input_buffer_),
                                    [this](error_code ec, Request &req){
                if(!ec){
                    std::string body(input_buffer_.begin(), input_buffer_.end());
                    req.ParseBody(body);
                }else{
                    std::cout << "Body Read failed, closing socket\n";
                    socket_.close();
                }
            });
        }

        void PassToController(Request &request){
            if(request.path[0] == 'U'){
                user_controller_.ControllerMapping::method_mapping_user.at(request.path.substr(1));
            }
        }

        tcp::socket socket_;
        boost::asio::io_context &context_;
        ThreadSafeQ<std::string> message_out_q_;
        ThreadSafeQ<std::string> &message_in_q_;
        std::vector<char> input_buffer_;
        Owner owner_;
        const std::string user_id;
        UserController &user_controller_;
        BidController &bid_controller_;
        TransactionController &transaction_controller_;



    };
}

#endif //SIMPLESTOCKEXCHANGE_CONNECTION_HPP
