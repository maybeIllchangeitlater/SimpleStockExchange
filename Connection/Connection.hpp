#ifndef SIMPLESTOCKEXCHANGE_CONNECTION_HPP
#define SIMPLESTOCKEXCHANGE_CONNECTION_HPP
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include "../Utility/ThreadSafeQ.hpp"
namespace s21 {
class Connection : public boost::enable_shared_from_this<Connection>{
    using tcp = boost::asio::ip::tcp;
    using connection_ptr = boost::shared_ptr<Connection>;
    using error_code = boost::system::error_code;
    public:
    enum Owner{
        SERVER,
        CLIENT,
    };

    Connection(Owner owner, boost::asio::io_context &context, tcp::socket &&socket, ///on tsq type?
               ThreadSafeQ<std::pair<connection_ptr, std::string>> &inc) :
               owner_(owner), context_(context), socket_(std::move(socket)), in_(inc){
    }

    void ConnectToClient();

    void ConnectToServer(const tcp::resolver::results_type& endpoints);

    void Disconnect(){
        if (Connected())
            boost::asio::post(context_, [this]() { socket_.close(); });
    }

    bool Connected() const{
        return socket_.is_open();
    }

    void Send(const std::string &msg);

    void Write();

    void Read();


    const std::string &GetUserId(){
        return user_id_;
    }


private:

    void AddToIncomingQueue();


    Owner owner_;
    boost::asio::io_context &context_;
    tcp::socket socket_;
    ThreadSafeQ<std::string> out_;
    ThreadSafeQ<std::pair<connection_ptr, std::string>> &in_;
    std::string unfinished_message_;
    std::string user_id_;
    bool authorized_;


    };
} //s21

#endif //SIMPLESTOCKEXCHANGE_CONNECTION_HPP
