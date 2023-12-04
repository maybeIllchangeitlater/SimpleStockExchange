#ifndef SIMPLESTOCKEXCHANGE_HANDLERS_HPP_
#define SIMPLESTOCKEXCHANGE_HANDLERS_HPP_

#include <boost/asio.hpp>
#include "../Server/Session.hpp"

namespace s21{
    constexpr static unsigned short kPort = 5050;

    class Handlers{
        using AcceptClientCallback = std::function<void(boost::shared_ptr<Session>)>;

    public:

        Handlers(boost::asio::io_context &context, AcceptClientCallback &&callback)
        : acceptor_(context,boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), kPort)),
        callback_(std::move(callback)){}

        void HandleAccept(boost::shared_ptr<Session> client, const boost::system::error_code &error,
                          boost::asio::io_context& context){
            callback_(client);
            client->Run();
            auto new_client = Session::MakeShared(context, callback_);
            acceptor_.async_accept(new_client->GetSocket(),
                                   boost::bind(&Handlers::HandleAccept, this, new_client, _1, _2));
        }

        boost::asio::ip::tcp::acceptor &GetAcceptor() {
            return acceptor_;
        }
    private:
        boost::asio::ip::tcp::acceptor acceptor_;
        AcceptClientCallback callback_;


    };
}

#endif //SIMPLESTOCKEXCHANGE_HANDLERS_HPP_
