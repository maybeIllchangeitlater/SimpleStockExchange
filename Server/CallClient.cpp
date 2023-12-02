#include "CallClient.hpp"

namespace s21 {

    void CallClient::Run() {
        {
            scopedlock lock(kClients_lock);
            kClients.emplace_back(shared_from_this());
        }
        scopedlock lock(client_lock_);
        running_ = true;
        last_ping_ = boost::posix_time::microsec_clock::local_time();
        Read();
    }

    void CallClient::Stop() {
        {
            scopedlock lock(client_lock_);
            if (!running_)
                return;
            running_ = false;
            socket_.close();
        }

        auto self = shared_from_this();
        {
            scopedlock lock(kClients_lock);
            auto it = std::find(kClients.begin(), kClients.end(), self);
            kClients.erase(it);
        }
        ///update;
    }

    boost::shared_ptr<CallClient> CallClient::MakeShared() {
        return boost::shared_ptr<CallClient>(new CallClient);
    }

    bool CallClient::Running() {
        scopedlock lock(client_lock_);
        return running_;
    }

    boost::asio::ip::tcp::socket &CallClient::GetSocket() {
        scopedlock lock(client_lock_);
        return socket_;
    }

    const std::string &CallClient::GetUserId() const {
        scopedlock lock(client_lock_);
        return user_id_;
    }

    void CallClient::SetClientChanged() {
        scopedlock lock(client_lock_);
        client_changed_ = true;
    }

    void CallClient::Read() {
        boost::asio::async_read(socket_, boost::asio::buffer(read_buffer_),
                                Binder::BindTo(*this, &self::ReadComplete, _1, _2), Binder::BindTo(*this, &self::ReadComplete, _1, _2));
        PostCheckPing();
    }

    void CallClient::OnRead(const boost::system::error_code &error, size_t bytes) {
        if (error)
            Stop();
        if (!Running())
            return;

        scopedlock lock(client_lock_);
        std::string message(read_buffer_.data(), bytes);

        if (message.find("login ") == 0) OnLogin(message);
        else if (message.find("ping") == 0) OnPing();
//        else if (message.find("ask_clients") == 0); //OnClients
//        else if (message.find("transactions") == 0); //OnTransactions
//        else if (message.find("rates") == 0); //OnRate
//        else; //throw or something
    }

    bool CallClient::ReadComplete(const boost::system::error_code &error, size_t bytes) {
        if(error)
            return false;
        return std::find(read_buffer_.data(), read_buffer_.data() + bytes, '\n') >= read_buffer_.data() + bytes;
    }

    void CallClient::Write(const std::string &message) {
        if(!Running())
            return;
        scopedlock lock(client_lock_);
        std::copy(message.begin(), message.end(), write_buffer_.begin());
        socket_.async_write_some(boost::asio::buffer(write_buffer_, message.size()), //mb just write buffer is enuf
                                 Binder::BindTo(*this, &self::OnWrite, _1, _2));
    }

    void CallClient::OnLogin(const std::string &message) {
        scopedlock lock(client_lock_);
        //login and password
        //incorrect, if correct - //Write("login successful")
        Write("login successful");
    }

    void CallClient::OnPing() {
        scopedlock lock(client_lock_);
        Write(client_changed_ ? "Client changed \n" : "Ping \n");
        client_changed_ = false;
    }

    void CallClient::OnCheckPing() {
        scopedlock lock(client_lock_);
        if((boost::posix_time::microsec_clock::local_time() - last_ping_).total_milliseconds() > kInactivityDeadline)
            Stop();
        last_ping_ = boost::posix_time::microsec_clock::local_time();
    }

    void CallClient::PostCheckPing() {
        scopedlock lock(client_lock_);
        deadline_timer_.expires_from_now(boost::posix_time::millisec(kInactivityDeadline));
        deadline_timer_.async_wait(Binder::BindTo(*this, &self::OnCheckPing));
    }



}