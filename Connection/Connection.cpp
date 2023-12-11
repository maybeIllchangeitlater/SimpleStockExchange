#include "Connection.hpp"
namespace s21{

    void Connection::ConnectToClient(){
        if (owner_ == SERVER){
            if (Connected()){
                ReadHeader();
        }
    }
    }

    void Connection::ConnectToServer(const tcp::resolver::results_type &endpoints){
        if(owner_ == CLIENT){
            boost::asio::async_connect(socket_, endpoints,
                                       [this](error_code ec, tcp::endpoint endpoint) {
                                           if (!ec) {
                                               ReadHeader();
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

    void Connection::ReadHeader(){
        boost::asio::async_read_until(socket_, boost::asio::dynamic_buffer(unfinished_message_), "\r\n\r\n",
                                      [this](error_code ec, std::size_t length)
                                      {
                                          std::cout << "I'm reading\n";
                                          if (!ec)
                                          {
                                              size_t body_length = FindBodyLength(unfinished_message_);
                                              std::cout << "BodyLength is :" << std::to_string(body_length)
                                              << "\nHeader is :" << unfinished_message_ << "\n";
                                              std::cout << "finished reading header\n";
                                              if(!body_length)
                                                AddToIncomingQueue();
                                              else{
                                                  ReadBody(body_length);
                                              }
                                              AddToIncomingQueue();
                                          }
                                          else
                                          {
                                              std::cout << "Read Header Fail.\n";
                                              socket_.close();
                                          }
                                      });
    }

    void Connection::ReadBody(size_t body_length) {
        unfinished_message_.resize(unfinished_message_.size() + body_length);
        boost::asio::async_read(socket_, boost::asio::buffer(unfinished_message_.data() + unfinished_message_.size() - body_length,
                                                             body_length), [this](error_code ec, size_t length){
            std::cout << "Reading Body\n";
            if(!ec){
                AddToIncomingQueue();
            }else{
                std::cout << "Read Body Fail.\n";
                socket_.close();
            }
        });
    }

    size_t Connection::FindBodyLength(const std::string &header) {
        std::string len_tag = "Content-Length: ";
        auto pos = header.find(len_tag);
        if(pos != std::string::npos){
            pos += len_tag.length();
            return std::stoul(header.substr(pos));
        }else{
            return 0;
        }
    }

    void Connection::AddToIncomingQueue(){
        std::cout << "Adding " << unfinished_message_ << " to incoming queue\n";
        if(owner_ == SERVER)
            in_.EmplaceBack(std::make_pair(shared_from_this(), std::move(unfinished_message_)));
        else {
            in_.EmplaceBack(std::make_pair(nullptr, std::move(unfinished_message_)));
        }
        ReadHeader();
    }
}