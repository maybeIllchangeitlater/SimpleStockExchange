#ifndef SIMPLESTOCKEXCHANGE_CLIENT_HPP
#define SIMPLESTOCKEXCHANGE_CLIENT_HPP

#include <memory>
#include <iostream>
#include "../Connection/Connection.hpp"
#include "../Controller/ClientController.hpp"
#include "../Utility/ThreadSafeQ.hpp"
#include "../Utility/ExtraJSONKeys.hpp"
#include <boost/make_unique.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

namespace s21 {
    class Client {
        using connection_ptr = boost::shared_ptr<Connection>;
        using tcp = boost::asio::ip::tcp;
    public:
        Client() = default;

        bool Connect(const std::string &host, short port);
        void Disconnect();
        bool Connected();
        void Send(const std::string &message);

        ThreadSafeQ<std::pair<connection_ptr, std::string>> & Inbox(){
            return from_server_;
        }

        void Register(const std::string &username, const std::string &password,
                      const std::string &balance_usd, const std::string &balance_rub){
            Send(ClientController::Register(username, password, balance_usd, balance_rub));
        }

        void LogIn(const std::string &username, const std::string& password){
            Send(ClientController::Login(username, password));
        }

        void CreateSellBid(const std::string &quantity, const std::string &rate){
            Send(ClientController::CreateBid(GetUserId(), quantity, rate, BidService::SELLING));
        }

        void CreateBuyBid(const std::string &quantity, const std::string &rate){
            Send(ClientController::CreateBid(GetUserId(), quantity, rate, BidService::BUYING));
        }

        void GetMySellBids(){
            Send(ClientController::GetMySellBids(GetUserId()));
        }

        void GetMyBuyBids(){
            Send(ClientController::GetMyBuyBids(GetUserId()));
        }

        void GetMySellTransactions(){
            Send(ClientController::GetMySellTransactions(GetUserId()));
        }

        void GetMyBuyTransactions(){
            Send(ClientController::GetMyBuyTransactions(GetUserId()));
        }

        void CancelBid(const std::string &bid_id){
            Send(ClientController::CancelBid(bid_id, user_id_));
        }

        void ChangeName(){
            std::string name;
            std::cout << "Please enter your new username\n";
            std::cin >> name;
            Send(ClientController::ChangeUserName(GetUserId(), name));
        }

        void ChangePassword(){
            std::string password;
            std::cout << "Please enter your new password\n";
            std::cin >> password;
            Send(ClientController::ChangePassword(GetUserId(), password));
        }

        void DeleteMe(){
            Send(ClientController::DeleteMe(GetUserId()));
            while (from_server_.Empty()) {}
            Disconnect();
        }

        void UpdateBidRate(const std::string &bid_id, const std::string &new_rate){
            Send(ClientController::UpdateBidRate(bid_id, new_rate));
        }

        void UpdateBidQuantity(const std::string &bid_id, const std::string &new_quantity){
            Send(ClientController::UpdateBidQuantity(bid_id, new_quantity));
        }

        void CheckBalance(){
            Send(ClientController::GetMyBalance(user_id_));
        }

        bool WaitForResponse(){
//            timer_->expires_from_now(boost::posix_time::seconds(3));
//                    timer_->async_wait([&](const boost::system::error_code&) {
//                            CutConnection();
//                            return false;
//                    });
            while (from_server_.Empty()) {}
//            timer_->cancel();
            return true;
        }
        bool CheckStatus(){
            return from_server_.Front().second.find("OK") != std::string::npos;
        }

        void Authorize(){
            auto msg = from_server_.PopFront().second;
            size_t json_start = msg.find('{');
            size_t json_end = msg.rfind('}');
            std::string json_str = msg.substr(json_start, json_end - json_start + 1);
            nlohmann::json json = nlohmann::json::parse(json_str);
            user_id_ = json["id"];
        }

        std::string CleanServerResponse(){
            auto msg = from_server_.PopFront().second;
            std::cout << msg << "\n";
            bool error = msg.find("message") != std::string::npos;
                size_t json_start = error
                        ? msg.find_last_of('{')
                        : msg.find('{');

                size_t json_end = error
                        ? msg.find('}')
                        : msg.rfind('}');
                return msg.substr(json_start +1 , json_end - json_start - 1);
        }

        bool CheckIfTransactionsWereMade() const noexcept{
            return from_server_.Front().second.find(s21::ExtraJSONKeys::buy_transaction) != std::string::npos
                    ||from_server_.Front().second.find(s21::ExtraJSONKeys::sell_transaction) != std::string::npos;
        }

        bool CheckIfTransactionsWereMade(const std::string &msg) const noexcept{
            return msg.find(s21::ExtraJSONKeys::buy_transaction) != std::string::npos
                    ||msg.find(s21::ExtraJSONKeys::sell_transaction) != std::string::npos;
        }

        void CutConnection(){
            context_->stop();
            if(thread_context_.joinable())
                thread_context_.join();
            connection_->Disconnect();
            connection_.reset();
            context_.reset();
            timer_.reset();
            from_server_.Clear();
        }

        std::string CleanBidCreatedResponse(std::string &msg){
            size_t first_part_start = msg.rfind(',');
            size_t second_part_start = msg.rfind('{');
            std::string first_part = msg.substr(first_part_start + 1, msg.rfind('}') - first_part_start - 1);
            std::string second_part = msg.substr(second_part_start + 1, msg.find(',') - second_part_start - 1);

            auto human_readable =  (first_part + second_part + " RUB per 1 USD");
            human_readable.
                    erase(std::remove_if(human_readable.begin(), human_readable.end(), [](char c) { return c == '\"' || c == ':'; }),
                          human_readable.end());

            return human_readable;
        }

        std::string ExtractDetailedBidInfoRaw(std::string &msg){
            std::cout << msg << "\t-\tbid creating detailed server info\n";
            if(msg.find(s21::BDNames::bid_table_create_update_time) == std::string::npos){
                return "";
            }
            size_t info_start = msg.rfind('{');
            size_t info_end = msg.rfind('}');
            auto detailed_info = msg.substr(info_start +1 , info_end - info_start - 1);
            msg.erase(info_start - 2, info_end);
            return detailed_info;

        }

        std::string ExtractCleanBidUpdateResponse(std::string &msg){
            std::string msg_bid_type = msg.find(s21::BDNames::missing_buyer) != std::string::npos
                    ? " sell "
                    : " buy ";

            std::string msg_bid = msg.substr(msg.find(s21::BDNames::bid_id_for_join) + 8,
                                      msg.find(',') - msg.find(s21::BDNames::bid_id_for_join) - 8);

            size_t quant_start_pos = msg.find(s21::BDNames::bid_table_quantity);
            std::string msg_quant = msg.substr(quant_start_pos, msg.find(',', quant_start_pos) - quant_start_pos);

            size_t rate_start_pos = msg.find(s21::BDNames::bid_table_rate);
            std::string msg_rate = msg.substr(rate_start_pos, msg.find(',', rate_start_pos) - rate_start_pos);

            std::string bid_update("Updated" + msg_bid_type +  "bid :\n" + msg_bid + "\nto " + msg_quant + " and " + msg_rate);
            bid_update.erase(std::remove(bid_update.begin(), bid_update.end(), '"'), bid_update.end());
            return bid_update;
        }




    private:
        const std::string &GetUserId(){
            return user_id_;
        }
        std::unique_ptr<boost::asio::io_context> context_;
        std::unique_ptr<boost::asio::deadline_timer> timer_;
        boost::thread thread_context_;
        std::unique_ptr<Connection> connection_;
        ThreadSafeQ<std::pair<connection_ptr, std::string>> from_server_;
        std::string user_id_;
    };
}

#endif //SIMPLESTOCKEXCHANGE_CLIENT_HPP
