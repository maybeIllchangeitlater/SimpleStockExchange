#include "Connection.hpp"

namespace s21{
    void Connection::Send(const std::string &message) {
        std::cout << "\nsending\n";
        boost::asio::post(context_, [this, message](){
            bool write_message = message_out_q_.Empty();
            message_out_q_.EmplaceBack(std::make_pair(shared_from_this() ,message));
            if(write_message){
                Write();
            }
        });
    }

    void Connection::Write() {
        boost::asio::async_write(socket_, boost::asio::buffer(message_out_q_.Front().second.data(),
                                                              message_out_q_.Front().second.size()),
                                 [this](error_code ec, size_t length){
                                     if(!ec){
                                         message_out_q_.PopFront();
                                         if(!message_out_q_.Empty()){
                                             Write();
                                         }
                                     }else{
                                         std::cout << "Body write failed, closing socket\n";
                                         socket_.close();
                                     }
                                 });
    }

    void Connection::ReadHeader() {
        std::cout << "reading header\n";
        boost::asio::async_read_until(socket_, boost::asio::dynamic_buffer(input_buffer_), "\r\n\r\n",
                                      [this](error_code ec, size_t length){
                                          if(!ec){
                                              std::string headers(input_buffer_.data(), input_buffer_.data() + length);
                                              unfinished_message_ = std::move(headers);
                                              ReadBody(FindContentLength(unfinished_message_), [this](std::string body) {
                                                  unfinished_message_ += body;
                                                  AddToInQueue();
                                              });
                                          }else{
                                              socket_.close();
                                          }
                                      });
    }

    void Connection::ReadBody(size_t body_length, std::function<void(std::string)> callback) {
        std::cout << "reading body\nBodylen is: " << body_length << std::endl;
        if(!body_length) callback("");
        input_buffer_.resize(body_length);
        boost::asio::async_read(socket_, boost::asio::buffer(input_buffer_),
                                [this, callback](error_code ec, size_t length){
                                    if(!ec){
                                        input_buffer_.resize(length);
                                        callback(std::string(input_buffer_.begin(), input_buffer_.end()));
                                    }else{
                                        std::cout << "Body Read failed, closing socket\n";
                                        socket_.close();
                                        callback("");
                                    }
                                });
    }
}