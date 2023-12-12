#include "Connection.hpp"
namespace s21{

    void Connection::ConnectToClient(){
        if (owner_ == SERVER){
            if (Connected()){
                Read();
        }
    }
    }

    void Connection::ConnectToServer(const tcp::resolver::results_type &endpoints){
        if(owner_ == CLIENT){
            boost::asio::async_connect(socket_, endpoints,
                                       [this](error_code ec, tcp::endpoint endpoint) {
                                           if (!ec) {
                                               Read();
                                           }
                                       });
        }
    }

    void Connection::Send(const std::string &msg) {
        std::cout << "I'm sending\n";
        boost::asio::post(context_,
                          [this, msg]() {
                              bool writing = !out_.Empty();
                              out_.EmplaceBack(msg);
                              if (!writing){
                                  Write();
                              }
                          });
    }

    void Connection::Write() {
        std::cout << "I'm writing\n";
        if(!out_.Empty()) {
                boost::asio::async_write(socket_, boost::asio::buffer(out_.Front().data(), out_.Front().length()),
                                         [this](error_code ec, size_t length) {
                                             if (!ec) {
                                                 std::cout << out_.PopFront() << "\n";
                                                 if (!out_.Empty()) {
                                                     Write();
                                                 }
                                             } else {
                                                 std::cout << "Write failed, closing socket\n";
                                             }
                                         });
        }
    }

    void Connection::Read(){
            boost::asio::async_read_until(socket_, boost::asio::dynamic_buffer(unfinished_message_), "\r\n\r\n",
                                          [this](error_code ec, std::size_t length) {
                                              std::cout << "I'm reading\n";
                                              if (!ec) {
                                                  AddToIncomingQueue();
                                              } else {
                                                  std::cout << "Read Header Fail.\n";
                                                  socket_.close();
                                              }
                                          });
    }

    void Connection::AddToIncomingQueue(){
        std::cout << "Adding " << unfinished_message_ << " to incoming queue\n";
        if(owner_ == SERVER)
            in_.EmplaceBack(std::make_pair(shared_from_this(), std::move(unfinished_message_)));
        else if(owner_ == CLIENT){
            in_.EmplaceBack(std::make_pair(nullptr, std::move(unfinished_message_)));
        }
        Read();
    }
}