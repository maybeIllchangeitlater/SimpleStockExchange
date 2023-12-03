#ifndef STOCKEXCHANGE_SERVER_CALLCLIENT_HPP_
#define STOCKEXCHANGE_SERVER_CALLCLIENT_HPP_

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <string>
#include "../Utility/Binder.hpp"

namespace s21 {
    class Session;
    constexpr static const size_t kInactivityDeadline = 7000;
    using StopCallback = std::function<void(boost::shared_ptr<Session>)>;

    class Session : public boost::enable_shared_from_this<Session> {
    private:
        using scopedlock = boost::recursive_mutex::scoped_lock;
        using self = Session;

        Session(boost::asio::io_context &context, StopCallback &&stop_callback)
        : deadline_timer_(context), socket_(context), stop_callback_(std::move(stop_callback)),
        running_(false), client_changed_(false) {}

        Session(const Session &) = delete;


    public:
        void Run();

        void Stop();

        static boost::shared_ptr<Session> MakeShared(boost::asio::io_context &context, StopCallback & stop_callback);

        bool Running();

        boost::asio::ip::tcp::socket &GetSocket();

        const std::string &GetUserId() const;

        void ChangeClient(const std::string& user_id) { user_id_ = user_id; }

        void SetClientChanged();

    private:
        void Read();

        void OnRead(const boost::system::error_code &error, size_t bytes);

        bool ReadComplete(const boost::system::error_code &error, size_t bytes);

        void Write(const std::string& message);

        void OnPing();
        ///poorly unimplemented:
        void Ping() { Write("ping"); }

        void OnWrite(const boost::system::error_code &, size_t) { Read(); } //poor iml, chec k err

        void OnLogin(const std::string &message);


        void OnCheckPing();

        void PostCheckPing();

        //client call ? mb in different place
        enum {
            kMaxLength = 1024
        };
        boost::array<char, kMaxLength> read_buffer_;
        boost::array<char, kMaxLength> write_buffer_;
        boost::asio::deadline_timer deadline_timer_;
        boost::asio::ip::tcp::socket socket_;
        mutable boost::recursive_mutex client_lock_;
        StopCallback stop_callback_;
        std::string user_id_;
        boost::posix_time::ptime last_ping_;
        bool running_;
        bool client_changed_;
    };
//    void ChangeClient(); //?
    // ручки в отдельный класс
} //s21

#endif //STOCKEXCHANGE_SERVER_CALLCLIENT_HPP_