#include <string>
#include <boost/test/unit_test.hpp>
#include <gmock/gmock.h>
#include "../Service/BalanceService.hpp"
#include "../Service/BidService.hpp"
#include "../Service/TransactionService.hpp"
#include "../Service/UserService.hpp"
#include "../Repository/BdNames.hpp"
#include "../Repository/BalanceRepository.hpp"
#include "../Repository/BidRepository.hpp"
#include "../Repository/UserRepository.hpp"
#include "../Repository/TransactionRepository.hpp"

namespace s21{
    class MockDatabases : public DatabaseInterface{
    public:
        MOCK_METHOD(pqxx::result, Execute, (const std::string &sql), (override));
        MOCK_METHOD(void, ExecuteAndCommit, (const std::string &sql), (override));
    };
}

struct BalanceServiceTO{
    testing::NiceMock<s21::MockDatabases> mock_db;
    s21::BalanceRepository bal_repo;
    s21::BalanceService bal_serv;
    std::string user_id = "1";
    std::string user_balance_usd = "22";
    std::string user_balance_rub = "222";
    BalanceServiceTO() : bal_repo(mock_db), bal_serv(bal_repo){}
};

struct TransactionServiceTO{
    testing::NiceMock<s21::MockDatabases> mock_db;
    s21::BalanceRepository bal_repo;
    s21::BalanceService bal_serv;
    s21::TransactionRepository trans_repo;
    s21::TransactionService trans_serv;
    std::string transaction_id = "1";
    std::string user_id = "11";
    std::string seller_id = "seller";
    std::string buyer_id = "buyer";
    std::string rate = "200";
    std::string quantity = "1";
    TransactionServiceTO() : bal_repo(mock_db), bal_serv(bal_repo),
                             trans_repo(mock_db), trans_serv(trans_repo, bal_serv){}
};

struct UserServiceTO{
    testing::NiceMock<s21::MockDatabases> mock_db;
    s21::BalanceRepository bal_repo;
    s21::BalanceService bal_serv;
    s21::UserRepository user_repo;
    s21::UserService user_serv;
    std::string user_id = "1";
    std::string username = "aboba";
    std::string bad_username = "1";
    std::string password = "Biba123";
    std::string bad_password = "123";
    std::string new_username = "biba";
    std::string new_password = "Boba321";
    std::string user_balance_usd = "22";
    std::string user_balance_rub = "222";
    std::string bad_balance = "a";
    UserServiceTO() : bal_repo(mock_db), bal_serv(bal_repo), user_repo(mock_db),
    user_serv(user_repo, bal_serv){}
};

struct BidServiceTO{
    testing::NiceMock<s21::MockDatabases> mock_db;
    s21::BalanceRepository bal_repo;
    s21::BalanceService bal_serv;
    s21::BidRepository bid_repo;
    s21::TransactionRepository trans_repo;
    s21::TransactionService trans_serv;
    s21::BidService bid_serv;
    std::string user_id = "1";
    std::string bid_id = "12";
    std::string rate = "500";
    std::string bad_rate = "aboba";
    std::string quantity = "222";
    std::string bad_quantity = "biba";
    BidServiceTO() : bal_repo(mock_db), bal_serv(bal_repo), bid_repo(mock_db),
    trans_repo(mock_db), trans_serv(trans_repo, bal_serv),
    bid_serv(bid_repo, trans_serv) {}
};

BOOST_FIXTURE_TEST_SUITE(BalanceServiceTests, BalanceServiceTO)

BOOST_AUTO_TEST_CASE(CreateUserBalance) {
        BOOST_CHECK_NO_THROW(
                                 bal_repo.CreateUserBalance(user_id, user_balance_usd, user_balance_rub);
                             );

}

BOOST_AUTO_TEST_CASE(GetUserBalance) {
        BOOST_CHECK_THROW({
            bal_repo.GetUserBalance(user_id);
        }, std::runtime_error);
}

BOOST_AUTO_TEST_CASE(SetUserBalance) {
        BOOST_CHECK_NO_THROW(
                bal_repo.SetUserBalance(user_id, user_balance_usd, user_balance_rub);
                );
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(BidServiceTests, BidServiceTO)

BOOST_AUTO_TEST_CASE(CreateBid_BadRate){
        BOOST_CHECK_THROW({
                              bid_serv.CreateBid(user_id, bad_rate, quantity, s21::BidService::BidType::SELLING);
                          }, std::logic_error);
}

BOOST_AUTO_TEST_CASE(CreateBid_BadQuantity){
        BOOST_CHECK_THROW({
                              bid_serv.CreateBid(user_id, rate, bad_quantity, s21::BidService::BidType::BUYING);
                          }, std::logic_error);
}

BOOST_AUTO_TEST_CASE(CreateBid){
        BOOST_CHECK_NO_THROW(
                bid_serv.CreateBid(user_id, rate, quantity, s21::BidService::BidType::SELLING);
                );
}

BOOST_AUTO_TEST_CASE(UpdateBidRate_BadRate){
        BOOST_CHECK_THROW({
            bid_serv.UpdateBidRate(bid_id, bad_rate);
        }, std::logic_error);
}

BOOST_AUTO_TEST_CASE(UpdateBidRate){
        BOOST_CHECK_THROW({
                              bid_serv.UpdateBidRate(bid_id, rate);
                          }, pqxx::argument_error);
}

BOOST_AUTO_TEST_CASE(UpdateBidQuantity_BadQuantity){
        BOOST_CHECK_THROW({
            bid_serv.UpdateBidQuantity(bid_id, bad_quantity);
        }, std::logic_error);
}

BOOST_AUTO_TEST_CASE(UpdateBidQuantity){
        BOOST_CHECK_NO_THROW(
                              bid_serv.UpdateBidQuantity(bid_id, quantity);
                          );
}

BOOST_AUTO_TEST_CASE(ReadBid){
        BOOST_CHECK_THROW({
                              bid_serv.ReadBid(bid_id);
                          }, pqxx::argument_error);
}

BOOST_AUTO_TEST_CASE(ReadAllUserSellBids){
        BOOST_CHECK_NO_THROW(
                              bid_serv.ReadAllUserSellBids(user_id);
                          );
}

BOOST_AUTO_TEST_CASE(ReadAllUserBuyBids){
    BOOST_CHECK_NO_THROW(
        bid_serv.ReadAllUserBuyBids(user_id);
    );
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(UserServiceTests, UserServiceTO)

BOOST_AUTO_TEST_CASE(CreateUser_BadName){
        BOOST_CHECK_THROW({
                              user_serv.CreateUser(bad_username, password, user_balance_usd, user_balance_rub);
                          }, std::logic_error);
}

BOOST_AUTO_TEST_CASE(CreateUser_BadPassword){
        BOOST_CHECK_THROW({
                              user_serv.CreateUser(username, bad_password, user_balance_usd, user_balance_rub);
                          }, std::logic_error);
}

BOOST_AUTO_TEST_CASE(CreateUser_BadBalance){
        BOOST_CHECK_THROW({
                              user_serv.CreateUser(username, password, bad_balance, user_balance_rub);
                          }, std::logic_error);
}

BOOST_AUTO_TEST_CASE(CreateUser){
        BOOST_CHECK_THROW({
                              user_serv.CreateUser(username, password, user_balance_usd, user_balance_rub);
                          }, std::runtime_error);
}

BOOST_AUTO_TEST_CASE(GetUserByName){
        BOOST_CHECK_THROW({
                              user_serv.GetUserByName(username);
                          }, std::runtime_error);
}

BOOST_AUTO_TEST_CASE(GetUserById){
        BOOST_CHECK_THROW({
                              user_serv.GetUserById(user_id);
                          }, pqxx::argument_error);
}

BOOST_AUTO_TEST_CASE(UpdateUserName_BadName){
        BOOST_CHECK_THROW({
                              user_serv.UpdateUserName(user_id, bad_username);
                          }, std::logic_error);
}

BOOST_AUTO_TEST_CASE(UpdateUserName){
        BOOST_CHECK_NO_THROW(
                              user_serv.UpdateUserName(user_id, username);
                          );
}

BOOST_AUTO_TEST_CASE(UpdateUserBalance_BadBalance){
        BOOST_CHECK_THROW({
                              user_serv.UpdateUserBalance(user_id, bad_balance, user_balance_rub);
        }, std::logic_error);
}

BOOST_AUTO_TEST_CASE(UpdateUserBalance){
        BOOST_CHECK_NO_THROW(
            user_serv.UpdateUserBalance(user_id, user_balance_usd, user_balance_rub);
        );
}

BOOST_AUTO_TEST_CASE(UpdateUserPassword_BadPassword){
        BOOST_CHECK_THROW({
                              user_serv.UpdateUserPassword(user_id, bad_password, password);
                          }, std::logic_error);
}

BOOST_AUTO_TEST_CASE(DeleteUser){
        BOOST_CHECK_THROW({
                              user_serv.DeleteUser(user_id, password);
                          }, pqxx::argument_error);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(TransactionServiceTests, TransactionServiceTO)

BOOST_AUTO_TEST_CASE(MakeTransaction){
        BOOST_CHECK_THROW({
                              trans_serv.MakeTransaction(seller_id, buyer_id, rate, quantity);
                          }, std::runtime_error);
}

BOOST_AUTO_TEST_CASE(ReadTransaction){
        BOOST_CHECK_THROW({
                              trans_serv.ReadTransaction(transaction_id);
                          }, std::runtime_error);
}

BOOST_AUTO_TEST_CASE(ReadAllUserSellTransactions){
        BOOST_CHECK_NO_THROW(
            trans_serv.ReadAllUserSellTransactions(user_id);
        );
}

BOOST_AUTO_TEST_CASE(ReadAllUserBuyTransactions){
        BOOST_CHECK_NO_THROW(
            trans_serv.ReadAllUserBuyTransactions(user_id);
        );
}

BOOST_AUTO_TEST_CASE(GetQuotations){
        BOOST_CHECK_NO_THROW(
            trans_serv.GetQuotations("7");
        );
}

BOOST_AUTO_TEST_SUITE_END()