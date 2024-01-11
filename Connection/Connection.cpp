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
                                       [this](error_code ec, tcp::endpoint) {
                                           if (!ec) {
                                               ReadHeader();
                                           }
                                       });
        }
    }

    void Connection::Send(const std::string &msg) {
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
        if(!out_.Empty()) {
                boost::asio::async_write(socket_, boost::asio::buffer(out_.Front().data(), out_.Front().length()),
                                         [this](error_code ec, size_t) {
                                             if (!ec) {
                                                 std::cout << out_.PopFront() << "\n";
                                                 if (!out_.Empty()) {
                                                     Write();
                                                 }
                                             } else {
                                                 std::cout << "Write failed, closing socket\n";
                                                 socket_.close();
                                             }
                                         });
        }
    }


    void Connection::ReadHeader() {
            boost::asio::async_read_until(socket_, boost::asio::dynamic_buffer(unfinished_message_), "\r\n\r\n",
                                          [this](error_code ec, std::size_t) {
                                              if (!ec) {
                                                  auto body_size = CalculateBodyLength();
                                                  if (body_size > 0) {
                                                      unfinished_message_.resize(
                                                              unfinished_message_.length() + body_size);
                                                      ReadBody(body_size);
                                                  } else {
                                                      AddToIncomingQueue();
                                                  }
                                              } else {
                                                  std::cout << "Read Header Fail.\n";
                                                  socket_.close();
                                              }
                                          });
    }

    void Connection::ReadBody(size_t body_length) {
        boost::asio::async_read(socket_,
                                boost::asio::buffer(unfinished_message_.data()
                                + unfinished_message_.length() - body_length, body_length),
                                [this](error_code ec, std::size_t) {
                                    if (!ec) {
                                        AddToIncomingQueue();
                                    }
                                    else {
                                        std::cout << "Read Body Fail.\n";
                                        socket_.close();
                                    }
                                });
    }

    size_t Connection::CalculateBodyLength() {
        auto delimiter_pos = unfinished_message_.find("\r\n\r\n");
        auto body_start = delimiter_pos + 4;
        auto content_length_pos = unfinished_message_.find("Content-Length:");
        auto content_length = std::stoul(unfinished_message_.substr(content_length_pos + 16));
        return content_length - (unfinished_message_.length() - body_start);
    }


    void Connection::AddToIncomingQueue(){
        if(owner_ == SERVER)
            in_.EmplaceBack(std::make_pair(shared_from_this(), std::move(unfinished_message_)));
        else if(owner_ == CLIENT){
            in_.EmplaceBack(std::make_pair(nullptr, std::move(unfinished_message_)));
        }
        ReadHeader();
    }
}