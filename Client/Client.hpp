#ifndef SIMPLESTOCKEXCHANGE_CLIENT_CLIENT_HPP
#define SIMPLESTOCKEXCHANGE_CLIENT_CLIENT_HPP

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread.hpp>
#include "../Utility/Binder.hpp"


namespace s21 {
class Client : public boost::enable_shared_from_this<Client> {
public:
        explicit Client(const std::string user_id, boost::asio::ip::tcp::socket &socket)
        : user_id_(user_id), socket_(socket){}

        void Login(const std::string user_name)

private:
    const std::string user_id_;
    boost::asio::ip::tcp::socket &socket_;

    };
}

#endif //SIMPLESTOCKEXCHANGE_CLIENT_CLIENT_HPP
