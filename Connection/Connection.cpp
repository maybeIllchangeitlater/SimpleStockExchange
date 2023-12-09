#include "Connection.hpp"

namespace s21{
    void Connection::Send(const std::string &message) {
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
        boost::asio::async_read_until(socket_, boost::asio::dynamic_buffer(input_buffer_), "\r\n\r\n",
                                      [this](error_code ec, size_t length){
                                          if(!ec){
                                              std::string headers(input_buffer_.data(), input_buffer_.data() + length);
                                              unfinished_message_ = std::move(headers);
                                              unfinished_message_ += ReadBody(FindContentLength(headers));
                                              AddToInQueue();
                                          }else{
                                              std::cout << "Header Read failed, closing socket\n";
                                              socket_.close();
                                          }
                                      });
    }

    std::string Connection::ReadBody(size_t body_length) {
        if(!body_length) return "";
        input_buffer_.resize(body_length);
        boost::asio::async_read(socket_, boost::asio::buffer(input_buffer_),
                                [this](error_code ec, size_t length){
                                    if(!ec){
                                        input_buffer_.resize(length);
                                        return std::string(input_buffer_.begin(), input_buffer_.end());
                                    }else{
                                        std::cout << "Body Read failed, closing socket\n";
                                        socket_.close();
                                        return std::string("");
                                    }
                                });
        return "";
    }
}