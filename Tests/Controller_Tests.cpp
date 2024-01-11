#include <string>
#include <boost/test/unit_test.hpp>
#include <gmock/gmock.h>
#include <iostream>
#include "../Controller/BidController.hpp"
#include "../Controller/TransactionController.hpp"
#include "../Controller/UserController.hpp"
#include "../Service/BidServiceInterface.hpp"
#include "../Service/BalanceServiceInterface.hpp"
#include "../Service/TransactionServiceInterface.hpp"
#include "../Service/UserServiceInterface.hpp"
#include "../Repository/BdNames.hpp"
#include "../Utility/ExtraJSONKeys.hpp"
#include "../Utility/Encoder.hpp"

using namespace s21;

namespace s21{
    class MockBidService : public BidServiceInterface{
    public:
        virtual ~MockBidService() = default;
        nlohmann::json CreateBid(const std::string &, const std::string& ,
                                 const std::string &, BidType ) override{
            nlohmann::json res;
            res[ExtraJSONKeys::bid_transaction_rate] = "123";
            res[ExtraJSONKeys::created_buy_bid_quantity] = "2";
            return res;
        }
        nlohmann::json ReadBid(const std::string &) override{
            nlohmann::json res;
            res[ExtraJSONKeys::bid_transaction_rate] = "123";
            res[ExtraJSONKeys::created_buy_bid_quantity] = "2";
            return res;
        }
        nlohmann::json UpdateBidRate(const std::string &, const std::string &) override{
            nlohmann::json res;
            res[ExtraJSONKeys::bid_transaction_rate] = "123";
            res[ExtraJSONKeys::created_buy_bid_quantity] = "2";
            return res;
        }
        void UpdateBidQuantity(const std::string &, const std::string &) override{
            return;
        }
        void CancelBid(const std::string &, const std::string &) override{
            return;
        }
        nlohmann::json ReadAllUserSellBids(const std::string &) override{
            nlohmann::json res;
            nlohmann::json res_tmp;
            res_tmp[ExtraJSONKeys::bid_transaction_rate] = "123";
            res_tmp[ExtraJSONKeys::created_buy_bid_quantity] = "2";
            res += res_tmp;
            res_tmp[ExtraJSONKeys::bid_transaction_rate] = "22";
            res_tmp[ExtraJSONKeys::created_buy_bid_quantity] = "33";
            res += res_tmp;
            return res;
        }
        nlohmann::json ReadAllUserBuyBids(const std::string &) override{
            nlohmann::json res;
            nlohmann::json res_tmp;
            res_tmp[ExtraJSONKeys::bid_transaction_rate] = "123";
            res_tmp[ExtraJSONKeys::created_buy_bid_quantity] = "2";
            res += res_tmp;
            res_tmp[ExtraJSONKeys::bid_transaction_rate] = "22";
            res_tmp[ExtraJSONKeys::created_buy_bid_quantity] = "33";
            res += res_tmp;
            return res;
        }
    };
    class MockTransactionService : public TransactionServiceInterface{
    public:
        virtual ~MockTransactionService() = default;
        void MakeTransaction(const std::string &, const std::string &,
                                         const std::string &, const std::string &) override{
                                             return;
                                         }
        nlohmann::json ReadTransaction(const std::string &) override{
            nlohmann::json res;
            res[BDNames::transaction_id_for_join] = "223";
            res[BDNames::joined_buyer_name] = "aboba";
            res[BDNames::joined_seller_name] = "biba";
            res[BDNames::transaction_table_rate] = "22";
            res[BDNames::transaction_table_quantity] = "23";
            res[BDNames::transaction_table_create_update_time] = "99:99:99";
            return res;
        }
        nlohmann::json ReadAllUserSellTransactions(const std::string &) override{
            nlohmann::json rres;
            nlohmann::json res;
            res[BDNames::transaction_id_for_join] = "223";
            res[BDNames::joined_buyer_name] = "aboba";
            res[BDNames::joined_seller_name] = "biba";
            res[BDNames::transaction_table_rate] = "22";
            res[BDNames::transaction_table_quantity] = "23";
            res[BDNames::transaction_table_create_update_time] = "99:99:99";
            rres += res;
            rres += res;
            return rres;
        }
        nlohmann::json ReadAllUserBuyTransactions(const std::string &) override{
            nlohmann::json rres;
            nlohmann::json res;
            res[BDNames::transaction_id_for_join] = "223";
            res[BDNames::joined_buyer_name] = "aboba";
            res[BDNames::joined_seller_name] = "biba";
            res[BDNames::transaction_table_rate] = "22";
            res[BDNames::transaction_table_quantity] = "23";
            res[BDNames::transaction_table_create_update_time] = "99:99:99";
            rres += res;
            rres += res;
            return rres;
        }

        nlohmann::json GetQuotations(const std::string &) override{
            nlohmann::json res;
            res[BDNames::transaction_id_for_join] = "223";
            res[BDNames::joined_buyer_name] = "aboba";
            res[BDNames::joined_seller_name] = "biba";
            res[BDNames::transaction_table_rate] = "22";
            res[BDNames::transaction_table_quantity] = "23";
            res[BDNames::transaction_table_create_update_time] = "99:99:99";
            return res;
        }

    };
    class MockUserService : public UserServiceInterface{
    public:
        virtual ~MockUserService() = default;
        void CreateUser(const std::string &, const std::string &,
                                const std::string &, const std::string &) override{
            return;
        }
        nlohmann::json GetUserByName(const std::string & name) override{
            nlohmann::json res;
            if(name == "user_name"){
                res[BDNames::user_table_id] = "1";
                res[BDNames::user_table_user_name] = "user_name";
                res[BDNames::user_table_password] = Encoder::Encode("123");
            }
            return res;
        }

        nlohmann::json GetUserById(const std::string &) override{
            nlohmann::json res;
            res[BDNames::user_table_id] = "1";
            res[BDNames::user_table_user_name] = "user_name";
            res[BDNames::user_table_password] = Encoder::Encode("123");
            res[BDNames::balance_table_usd] = "0";
            res[BDNames::balance_table_rub] = "0";
            return res;
        }

        void UpdateUserName(const std::string &, const std::string &) override{
            return;
        }

        void UpdateUserBalance(const std::string &, const std::string &,
                               const std::string &) override{
            return;
        }

        void UpdateUserPassword(const std::string &,
                                const std::string &, const std::string &) override{
            return;
        }

        void DeleteUser(const std::string &, const std::string &) override{
            return;
        }

    };
}

struct BidControllerTO{
    MockBidService mock_bids;
    BidController bid_controller;
    nlohmann::json request_body;

    BidControllerTO() : bid_controller(mock_bids){
        request_body[BDNames::bid_table_id] = "1";
        request_body[BDNames::bid_table_rate] = "123";
        request_body[BDNames::bid_table_type] = 2;
        request_body[BDNames::bid_table_quantity] = "2";
        request_body[BDNames::bid_table_buyer_id] = "228";
        request_body[BDNames::trader_id] = "12";
    }
};

struct TransactionControllerTO {
    MockTransactionService mock_transactions;
    TransactionController transaction_controller;
    nlohmann::json request_body;

    TransactionControllerTO() : transaction_controller(mock_transactions) {
        request_body[BDNames::transaction_table_id] = "1";
        request_body[BDNames::transaction_table_rate] = "123";
        request_body[BDNames::transaction_table_quantity] = "2";
        request_body[BDNames::transaction_table_buyer_id] = "228";
        request_body[BDNames::trader_id] = "12";
        request_body[ExtraJSONKeys::time_period] = "99:99:99";
    }
};

struct UserControllerTO{
    MockUserService mock_users;
    UserController user_controller;
    nlohmann::json request_body;

    UserControllerTO() : user_controller(mock_users){
        request_body[BDNames::user_table_id] = "1";
        request_body[BDNames::user_table_user_name] = "user_name";
        request_body[BDNames::user_table_password] = "123";
        request_body[ExtraJSONKeys::old_password] = "123";
        request_body[BDNames::balance_table_usd] = "0";
        request_body[BDNames::balance_table_rub] = "0";
    }
};

BOOST_FIXTURE_TEST_SUITE(BidController_Tests, BidControllerTO)

BOOST_AUTO_TEST_CASE(CreateBidTest){
    nlohmann::json response = bid_controller.CreateBid(request_body);
    BOOST_CHECK_EQUAL(response[ExtraJSONKeys::bid_transaction_rate], "123");
    BOOST_CHECK_EQUAL(response[ExtraJSONKeys::created_buy_bid_quantity], "2");
}

BOOST_AUTO_TEST_CASE(GetBidTest){
    nlohmann::json response = bid_controller.GetBid(request_body);
    BOOST_CHECK_EQUAL(response[ExtraJSONKeys::bid_transaction_rate], "123");
    BOOST_CHECK_EQUAL(response[ExtraJSONKeys::created_buy_bid_quantity], "2");
}

BOOST_AUTO_TEST_CASE(GetUserBuyBidsTest){
    nlohmann::json response = bid_controller.GetUserBuyBids(request_body);
    BOOST_CHECK_EQUAL(response[0][ExtraJSONKeys::bid_transaction_rate], "123");
    BOOST_CHECK_EQUAL(response[0][ExtraJSONKeys::created_buy_bid_quantity], "2");
    BOOST_CHECK_EQUAL(response[1][ExtraJSONKeys::bid_transaction_rate], "22");
    BOOST_CHECK_EQUAL(response[1][ExtraJSONKeys::created_buy_bid_quantity], "33");
}

BOOST_AUTO_TEST_CASE(GetUserSellBidsTest){
    nlohmann::json response = bid_controller.GetUserSellBids(request_body);
    BOOST_CHECK_EQUAL(response[0][ExtraJSONKeys::bid_transaction_rate], "123");
    BOOST_CHECK_EQUAL(response[0][ExtraJSONKeys::created_buy_bid_quantity], "2");
    BOOST_CHECK_EQUAL(response[1][ExtraJSONKeys::bid_transaction_rate], "22");
    BOOST_CHECK_EQUAL(response[1][ExtraJSONKeys::created_buy_bid_quantity], "33");
}

BOOST_AUTO_TEST_CASE(UpdateBidRateTest){
    nlohmann::json response = bid_controller.UpdateBidRate(request_body);
    BOOST_CHECK_EQUAL(response[ExtraJSONKeys::bid_transaction_rate], "123");
    BOOST_CHECK_EQUAL(response[ExtraJSONKeys::created_buy_bid_quantity], "2");
}

BOOST_AUTO_TEST_CASE(UpdateBidQuantityTest){
    nlohmann::json response = bid_controller.UpdateBidQuantity(request_body);
    BOOST_CHECK_EQUAL(response[ExtraJSONKeys::bid_transaction_rate], "123");
    BOOST_CHECK_EQUAL(response[ExtraJSONKeys::created_buy_bid_quantity], "2");
}

BOOST_AUTO_TEST_CASE(CancelBid){
    nlohmann::json response = bid_controller.CancelBid(request_body);
    BOOST_CHECK_EQUAL(response[ExtraJSONKeys::status], ServerMessage::OK);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(TransactionController_Tests, TransactionControllerTO)

BOOST_AUTO_TEST_CASE(GetQuotationsTest){
    nlohmann::json response = transaction_controller.GetQuotations(request_body);
    BOOST_CHECK_EQUAL(response[ExtraJSONKeys::status], ServerMessage::OK);
}

BOOST_AUTO_TEST_CASE(GetUserBuyTransactionsTest){
    nlohmann::json response = transaction_controller.GetUserBuyTransactions(request_body);
    BOOST_CHECK_EQUAL(response.back()[ExtraJSONKeys::status], ServerMessage::OK);
}

BOOST_AUTO_TEST_CASE(GetUserSellTransactionsTest){
    nlohmann::json response = transaction_controller.GetUserSellTransactions(request_body);
    BOOST_CHECK_EQUAL(response.back()[ExtraJSONKeys::status], ServerMessage::OK);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(UserController_Tests, UserControllerTO)

BOOST_AUTO_TEST_CASE(AuthenticateUserTest){
    nlohmann::json response = user_controller.AuthenticateUser(request_body);
    BOOST_CHECK_EQUAL(response[ExtraJSONKeys::status], ServerMessage::OK);
}

BOOST_AUTO_TEST_CASE(AuthenticateUserTest_BadPassword){
    request_body[BDNames::user_table_password] = "22";
    nlohmann::json response = user_controller.AuthenticateUser(request_body);
    BOOST_CHECK_EQUAL(response[ExtraJSONKeys::status], ServerMessage::BAD_REQUEST);
}

BOOST_AUTO_TEST_CASE(RegisterUser_AlreadyExists){
        nlohmann::json response = user_controller.RegisterUser(request_body);
        BOOST_CHECK_EQUAL(response[ExtraJSONKeys::status], ServerMessage::BAD_REQUEST);
}

BOOST_AUTO_TEST_CASE(RegisterUser){
        BOOST_CHECK_NO_THROW(nlohmann::json response = user_controller.RegisterUser(request_body););
}

BOOST_AUTO_TEST_CASE(GetUserById){
    nlohmann::json response = user_controller.GetUserById(request_body);
    BOOST_CHECK_EQUAL(response[ExtraJSONKeys::status], ServerMessage::OK);
}

BOOST_AUTO_TEST_CASE(GetUserByName){
    request_body[BDNames::user_table_user_name] = "user_name";
    nlohmann::json response = user_controller.GetUserByName(request_body);
    BOOST_CHECK_EQUAL(response[ExtraJSONKeys::status], ServerMessage::OK);
}

BOOST_AUTO_TEST_CASE(UpdateUserName){
    nlohmann::json response = user_controller.UpdateUserName(request_body);
    BOOST_CHECK_EQUAL(response[ExtraJSONKeys::status], ServerMessage::OK);
}

BOOST_AUTO_TEST_CASE(UpdateUserPassword){
    nlohmann::json response = user_controller.UpdateUserPassword(request_body);
    BOOST_CHECK_EQUAL(response[ExtraJSONKeys::status], ServerMessage::OK);
}

BOOST_AUTO_TEST_CASE(DeleteUser){
    nlohmann::json response = user_controller.DeleteUser(request_body);
    BOOST_CHECK_EQUAL(response[ExtraJSONKeys::status], ServerMessage::OK);
}

BOOST_AUTO_TEST_CASE(UpdateUserBalance){
    nlohmann::json response = user_controller.UpdateUserBalance(request_body);
    BOOST_CHECK_EQUAL(response[ExtraJSONKeys::status], ServerMessage::OK);
}

BOOST_AUTO_TEST_SUITE_END()