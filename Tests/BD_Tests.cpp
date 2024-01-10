#include <string>
#include <boost/test/unit_test.hpp>
#include <gmock/gmock.h>
#include "../3rdParty/libpqxx/include/pqxx/pqxx"
#include "../Repository/BdNames.hpp"
#include "../Repository/BalanceRepository.hpp"
#include "../Repository/BidRepository.hpp"
#include "../Repository/UserRepository.hpp"
#include "../Repository/TransactionRepository.hpp"
#include "../Repository/DatabaseInterface.hpp"

namespace s21{
    class MockDatabase : public DatabaseInterface{
    public:
        MOCK_METHOD(pqxx::result, Execute, (const std::string &sql), (override));
        MOCK_METHOD(void, ExecuteAndCommit, (const std::string &sql), (override));
    };
}

struct BalanceRepositoryTO {
    s21::MockDatabase mock_db;
    s21::BalanceRepository balance_repo;
    std::string user_id = "test_user";
    std::string balance_rub = "200";
    std::string balance_usd = "200";
    BalanceRepositoryTO() : balance_repo(mock_db) {}

};

struct TransactionRepositoryTO{
    s21::MockDatabase mock_db;
    s21::TransactionRepository trans_repo;
    std::string transaction_id = "1";
    std::string seller_id = "seller";
    std::string buyer_id = "buyer";
    std::string rate = "25";
    std::string quantity = "45";
    std::string time = "999:999:999";
    TransactionRepositoryTO() : trans_repo(mock_db) {}
};

struct UserRepositoryTO {
    s21::MockDatabase mock_db;
    s21::UserRepository user_repo;
    std::string user_id = "1";
    std::string username = "aboba";
    std::string password = "123";
    std::string new_username = "biba";
    std::string new_password = "321";
    UserRepositoryTO() : user_repo(mock_db){}
};

struct BidRepositoryTO {
    s21::MockDatabase mock_db;
    s21::BidRepository bid_repo;
    std::string user_id = "1";
    std::string seller_id = "seller";
    std::string buyer_id = "buyer";
    std::string rate = "500";
    std::string quantity = "300";
    std::string bid_id = "bid1";
    std::string time = "99:99:99";
    BidRepositoryTO() : bid_repo(mock_db){}
};
BOOST_FIXTURE_TEST_SUITE(BalanceRepositoryTests, BalanceRepositoryTO)

BOOST_AUTO_TEST_CASE(GetUserBalanceTest_NotFound_Exception) { //pqxx does not provide ability to add anything to it so will always throw
    pqxx::result mock_result;

    std::string sql = "SELECT " + std::string(s21::BDNames::balance_table_user_id)
                      + ", " + s21::BDNames::balance_table_usd
                      + ", " + s21::BDNames::balance_table_rub
                      + " FROM " + s21::BDNames::balance_table
                      + " WHERE " + s21::BDNames::balance_table_user_id
                      + " = '" + user_id + "'";
        BOOST_CHECK_THROW({
                              EXPECT_CALL(mock_db, Execute(sql))
                                      .WillOnce(testing::Return(mock_result));

                              auto result = balance_repo.GetUserBalance(user_id);
                          }, std::runtime_error);

}

BOOST_AUTO_TEST_CASE(CreateUserBalance) {
        std::string sql = "INSERT INTO " + std::string(s21::BDNames::balance_table)
                          + " (" + s21::BDNames::balance_table_user_id
                          + ", " + s21::BDNames::balance_table_usd
                          + ", " + s21::BDNames::balance_table_rub
                          + ") VALUES ('" + user_id
                          + "', " + balance_usd
                          + ", " + balance_rub + ");";

        BOOST_CHECK_NO_THROW({
                                 EXPECT_CALL(mock_db, ExecuteAndCommit(sql))
                                         .WillOnce(testing::Return());
                                 balance_repo.CreateUserBalance(user_id, balance_usd, balance_rub);
        });
}

BOOST_AUTO_TEST_CASE(SetUserBalance) {
        std::string sql = "UPDATE " + std::string(s21::BDNames::balance_table)
                          + " SET " + s21::BDNames::balance_table_usd
                          + " = '" + balance_usd + "', " + s21::BDNames::balance_table_rub
                          + " = '" + balance_rub
                          + "' WHERE " + std::string(s21::BDNames::balance_table_user_id)
                          + " = '" + user_id + "'";

        BOOST_CHECK_NO_THROW({
                                 EXPECT_CALL(mock_db, ExecuteAndCommit(sql))
                                         .WillOnce(testing::Return());
                                 balance_repo.SetUserBalance(user_id, balance_usd, balance_rub);
                             });
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(TransactionRepositoryTests, TransactionRepositoryTO)

BOOST_AUTO_TEST_CASE(CreateTransaction) {
        std::string sql = "INSERT INTO " + std::string(s21::BDNames::transaction_table) + " ("
                          + s21::BDNames::transaction_table_seller_id + ", "
                          + s21::BDNames::transaction_table_buyer_id + ", "
                          + s21::BDNames::transaction_table_rate + ", "
                          + s21::BDNames::transaction_table_quantity + ", "
                          + s21::BDNames::transaction_table_create_update_time
                          + ") VALUES ('" + seller_id + "', '" + buyer_id + "', '"
                          + rate + "', '" + quantity + "', '" + time + "')";
        BOOST_CHECK_NO_THROW({
                                 EXPECT_CALL(mock_db, ExecuteAndCommit(sql))
                                 .WillOnce(testing::Return());
                                 trans_repo.CreateTransaction(seller_id, buyer_id, rate, quantity, time);
        });
}

BOOST_AUTO_TEST_CASE(ReadTransaction_NotFound) {
        pqxx::result mock_res;
        std::string sql = "SELECT t.'" + std::string(s21::BDNames::transaction_table_id) + "' AS transaction id, "
                          + "seller.'" + s21::BDNames::user_table_user_name + "' AS seller name, "
                          + "buyer.'" + s21::BDNames::user_table_user_name + "' AS buyer name, "
                          + "t.'" + s21::BDNames::transaction_table_rate
                          + "', t.'" + s21::BDNames::transaction_table_quantity
                          + "', t.'" + s21::BDNames::transaction_table_create_update_time
                          + "' FROM '" + s21::BDNames::transaction_table + "' t "
                          + "JOIN '" + s21::BDNames::user_table
                          + "' seller ON t.'" + s21::BDNames::transaction_table_seller_id
                          + "' = seller.'" + s21::BDNames::user_table_id
                          + "' JOIN'" + s21::BDNames::user_table
                          + "' buyer ON t.'" + s21::BDNames::transaction_table_buyer_id
                          + "' = buyer.'" + s21::BDNames::user_table_id
                          + "' WHERE t.'" + s21::BDNames::transaction_table_id
                          + "' = '" + transaction_id + "'";
        BOOST_CHECK_THROW({
                              EXPECT_CALL(mock_db, Execute(sql))
                              .WillOnce(testing::Return(mock_res));
                              auto res = trans_repo.ReadTransaction(transaction_id);
        }, std::runtime_error);
}

BOOST_AUTO_TEST_CASE(ReadAllUserSellTransactions) {
        std::string sql = "SELECT b." + std::string(s21::BDNames::transaction_table_id)
                          + " AS " + s21::BDNames::transaction_id_for_join + ", seller."
                          + s21::BDNames::user_table_user_name +
                          " AS " + s21::BDNames::joined_seller_name
                          + ", buyer." + s21::BDNames::user_table_user_name
                          + " AS " + s21::BDNames::joined_buyer_name
                          + ", b." + s21::BDNames::transaction_table_rate
                          + ", b." + s21::BDNames::transaction_table_quantity
                          + ", b." + s21::BDNames::transaction_table_create_update_time
                          + " FROM " + s21::BDNames::transaction_table
                          + " b LEFT JOIN " + s21::BDNames::user_table + " seller ON b."
                          + s21::BDNames::bid_table_seller_id
                          + " = seller." + s21::BDNames::user_table_id
                          + " JOIN "  + s21::BDNames::user_table + " buyer ON b."
                          + s21::BDNames::bid_table_buyer_id
                          + " = buyer." + s21::BDNames::user_table_id
                          + " WHERE seller." + s21::BDNames::user_table_id
                          + " = '" + seller_id + "'";
        pqxx::result mock_res;
        BOOST_CHECK_NO_THROW({
                                 EXPECT_CALL(mock_db, Execute(sql))
                                 .WillOnce(testing::Return(mock_res));
                                 auto res = trans_repo.ReadAllUserSellTransactions(seller_id);
                                 BOOST_ASSERT(res == mock_res);
        });
}

BOOST_AUTO_TEST_CASE(ReadAllUserBuyTransactions) {
        std::string sql = "SELECT b." + std::string(s21::BDNames::transaction_table_id)
                          + " AS " + s21::BDNames::transaction_id_for_join
                          + ", seller." + s21::BDNames::user_table_user_name
                          + " AS " + s21::BDNames::joined_seller_name
                          + ", buyer." + s21::BDNames::user_table_user_name
                          + " AS " + s21::BDNames::joined_buyer_name
                          + ", b." + s21::BDNames::transaction_table_rate
                          + ", b." + s21::BDNames::transaction_table_quantity
                          + ", b." + s21::BDNames::transaction_table_create_update_time
                          + " FROM " + s21::BDNames::transaction_table
                          + " b JOIN " + s21::BDNames::user_table + " seller ON b."
                          + s21::BDNames::transaction_table_seller_id
                          + " = seller." + s21::BDNames::user_table_id
                          + " LEFT JOIN "  + s21::BDNames::user_table + " buyer ON b."
                          + s21::BDNames::transaction_table_buyer_id
                          + " = buyer." + s21::BDNames::user_table_id
                          + " WHERE buyer." + s21::BDNames::user_table_id
                          + " = '" + buyer_id + "'";
        pqxx::result mock_res;
        BOOST_CHECK_NO_THROW({
                                 EXPECT_CALL(mock_db, Execute(sql))
                                         .WillOnce(testing::Return(mock_res));
                                 auto res = trans_repo.ReadAllUserBuyTransactions(buyer_id);
                                 BOOST_ASSERT(res == mock_res);
                             });
}

BOOST_AUTO_TEST_CASE(GetAllTransactionsForLast) {
        std::string sql = "SELECT * FROM transaction_info WHERE " +
                          std::string(s21::BDNames::transaction_table_create_update_time) +  " >= CURRENT_DATE - INTERVAL '" +
                          time + " days'";
        pqxx::result mock_res;
        BOOST_CHECK_NO_THROW({
                                 EXPECT_CALL(mock_db, Execute(sql))
                                         .WillOnce(testing::Return(mock_res));
                                 auto res = trans_repo.GetTransactionsForLast(time);
                                 BOOST_ASSERT(res == mock_res);
                             });
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(UserRepositoryTests, UserRepositoryTO)

BOOST_AUTO_TEST_CASE(CreateUser) {
        std::string sql = "INSERT INTO " +  std::string(s21::BDNames::user_table) +
                          "(" + s21::BDNames::user_table_user_name + ", "
                          + s21::BDNames::user_table_password
                          + ") VALUES ('" +
                          username + "', '" + password + "');";
        BOOST_CHECK_NO_THROW({
                                 EXPECT_CALL(mock_db, ExecuteAndCommit(sql))
                                 .WillOnce(testing::Return());
                                 user_repo.CreateUser(username, password);
        });
}

BOOST_AUTO_TEST_CASE(ReadUserById) {
        std::string sql = "SELECT " + std::string(s21::BDNames::user_table_id)
                          + ", " + s21::BDNames::user_table_user_name
                          + ", " + s21::BDNames::user_table_password
                          + " FROM " + s21::BDNames::user_table
                          + " WHERE " + s21::BDNames::user_table_id
                          + " = '" + user_id +"'";
        pqxx::result mock_res;
        BOOST_CHECK_NO_THROW({
                                 EXPECT_CALL(mock_db, Execute(sql))
                                         .WillOnce(testing::Return(mock_res));
                                 auto res = user_repo.ReadUserById(user_id);
                                 BOOST_ASSERT(res == mock_res);
                             });
}

BOOST_AUTO_TEST_CASE(ReadUserByName_NotFound) {
        std::string sql = "SELECT " + std::string(s21::BDNames::user_table_id)
                          + ", " + s21::BDNames::user_table_user_name
                          + ", " + s21::BDNames::user_table_password
                          + " FROM " + s21::BDNames::user_table
                          + " WHERE " + s21::BDNames::user_table_user_name
                          + " = '" + username + "'";
        pqxx::result mock_res;
        BOOST_CHECK_THROW({
                              EXPECT_CALL(mock_db, Execute(sql))
                                      .WillOnce(testing::Return(mock_res));
                              user_repo.ReadUserByName(username);
        }, std::runtime_error);
}

BOOST_AUTO_TEST_CASE(UpdateUserName) {
        std::string sql = "UPDATE " + std::string(s21::BDNames::user_table)
                          + " SET " + s21::BDNames::user_table_user_name
                          + " = '" + new_username
                          + "' WHERE " + s21::BDNames::user_table_id
                          + " = '" + user_id + "'";
        BOOST_CHECK_NO_THROW({
                                 EXPECT_CALL(mock_db, ExecuteAndCommit(sql))
                                         .WillOnce(testing::Return());
                                 user_repo.UpdateUserName(user_id, new_username);
                             });
}

BOOST_AUTO_TEST_CASE(UpdateUserPassword) {
        std::string sql = "UPDATE " + std::string(s21::BDNames::user_table)
                          + " SET " + s21::BDNames::user_table_password
                          + " = '" + new_password
                          + "' WHERE " + s21::BDNames::user_table_id
                          + " = '" + user_id + "'";
        BOOST_CHECK_NO_THROW({
                                 EXPECT_CALL(mock_db, ExecuteAndCommit(sql))
                                         .WillOnce(testing::Return());
                                 user_repo.UpdateUserPassword(user_id, new_password);
                             });
}

BOOST_AUTO_TEST_CASE(DeleteUser) {
        std::string sql = "DELETE FROM " + std::string(s21::BDNames::user_table)
                          + " WHERE " + s21::BDNames::user_table_id
                          + " = '" + user_id + "'";
        BOOST_CHECK_NO_THROW({
                                 EXPECT_CALL(mock_db, ExecuteAndCommit(sql))
                                         .WillOnce(testing::Return());
                                 user_repo.DeleteUser(user_id);
                             });
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(BidRepositoryTests, BidRepositoryTO)

BOOST_AUTO_TEST_CASE(CreateSellBid) {
        std::string sql = "INSERT INTO " + std::string(s21::BDNames::bid_table)
                          + "(" + std::string(s21::BDNames::bid_table_seller_id)
                          + ", " + std::string(s21::BDNames::bid_table_rate)
                          + ", " + std::string(s21::BDNames::bid_table_quantity)
                          + ", " + std::string(s21::BDNames::bid_table_create_update_time)
                          + ") VALUES ('" + seller_id + "', '" + rate + "', '"
                          + quantity + "', '" + time + "')";
        BOOST_CHECK_NO_THROW({
                                 EXPECT_CALL(mock_db, ExecuteAndCommit(sql))
                                 .WillOnce(testing::Return());
                                 bid_repo.CreateSellBid(seller_id, rate, quantity, time);
        });
}

BOOST_AUTO_TEST_CASE(CreateBuyBid) {
        std::string sql = "INSERT INTO " + std::string(s21::BDNames::bid_table)
                          + "(" + std::string(s21::BDNames::bid_table_buyer_id)
                          + ", " + std::string(s21::BDNames::bid_table_rate)
                          + ", " + std::string(s21::BDNames::bid_table_quantity)
                          + ", " + std::string(s21::BDNames::bid_table_create_update_time)
                          + ") VALUES ('" + buyer_id
                          + "', '" + rate + "', '"
                          + quantity +  "', '" + time + "')";
        BOOST_CHECK_NO_THROW({
                                 EXPECT_CALL(mock_db, ExecuteAndCommit(sql))
                                         .WillOnce(testing::Return());
                                 bid_repo.CreateBuyBid(buyer_id, rate, quantity, time);
                             });
}

BOOST_AUTO_TEST_CASE(ReadBid) {
        std::string sql = "SELECT b." + std::string(s21::BDNames::bid_table_id)
                          + " AS " + s21::BDNames::bid_id_for_join
                          + ", seller." + s21::BDNames::user_table_user_name
                          + " AS " + s21::BDNames::joined_seller_name
                          + ", buyer." + s21::BDNames::user_table_user_name
                          + " AS " + s21::BDNames::joined_buyer_name
                          + ", b." + s21::BDNames::bid_table_rate
                          + ", b." + s21::BDNames::bid_table_quantity
                          + ", b." + s21::BDNames::bid_table_create_update_time
                          + " FROM " + s21::BDNames::bid_table
                          + " b LEFT JOIN " + s21::BDNames::user_table + " seller ON "
                          + s21::BDNames::bid_table_seller_id
                          + " = seller." + s21::BDNames::user_table_id
                          + " LEFT JOIN "  + s21::BDNames::user_table + " buyer ON "
                          + s21::BDNames::bid_table_buyer_id
                          + " = buyer." + s21::BDNames::user_table_id
                          + " WHERE b." + s21::BDNames::bid_table_id
                          + " = '" + bid_id + "'";
        pqxx::result mock_res;
        BOOST_CHECK_NO_THROW({
                                 EXPECT_CALL(mock_db, Execute(sql))
                                         .WillOnce(testing::Return(mock_res));
                                 auto res = bid_repo.ReadBid(bid_id);
                                 BOOST_ASSERT(res == mock_res);
                             });
}

BOOST_AUTO_TEST_CASE(ReadBidRaw) {
        std::string sql = "SELECT * FROM " + std::string(s21::BDNames::bid_table)
                          + " WHERE " + std::string(s21::BDNames::bid_table_id)
                          + " = '" + bid_id + "'";
        pqxx::result mock_res;
        BOOST_CHECK_NO_THROW({
                                 EXPECT_CALL(mock_db, Execute(sql))
                                         .WillOnce(testing::Return(mock_res));
                                 auto res = bid_repo.ReadBidRaw(bid_id);
                                 BOOST_ASSERT(res == mock_res);
                             });
}

BOOST_AUTO_TEST_CASE(ReadAllUserSellBids) {
        std::string sql = "SELECT b." + std::string(s21::BDNames::bid_table_id)
                          + " AS " + s21::BDNames::bid_id_for_join
                          + ", seller." + std::string(s21::BDNames::user_table_user_name)
                          + " AS " + s21::BDNames::joined_seller_name
                          + ", buyer." + std::string(s21::BDNames::user_table_user_name)
                          + " AS " + s21::BDNames::joined_buyer_name
                          + ", b." + std::string(s21::BDNames::bid_table_rate)
                          + ", b." + std::string(s21::BDNames::bid_table_quantity)
                          + ", b." + std::string(s21::BDNames::bid_table_create_update_time)
                          + " FROM " + std::string(s21::BDNames::bid_table)
                          + " b JOIN " + std::string(s21::BDNames::user_table) + " seller ON b."
                          + std::string(s21::BDNames::bid_table_seller_id)
                          + " = seller." + std::string(s21::BDNames::user_table_id)
                          + " LEFT JOIN "  + std::string(s21::BDNames::user_table) + " buyer ON b."
                          + std::string(s21::BDNames::bid_table_buyer_id)
                          + " = buyer." + std::string(s21::BDNames::user_table_id)
                          + " WHERE seller." + std::string(s21::BDNames::user_table_id)
                          + " = '" + seller_id + "'";
        pqxx::result mock_res;
        BOOST_CHECK_NO_THROW({
                                 EXPECT_CALL(mock_db, Execute(sql))
                                         .WillOnce(testing::Return(mock_res));
                                 auto res = bid_repo.ReadAllUserSellBids(seller_id);
                                 BOOST_ASSERT(res == mock_res);
                             });
}

BOOST_AUTO_TEST_CASE(ReadAllUserBuyBids) {
        std::string sql = "SELECT b." + std::string(s21::BDNames::bid_table_id)
                          + " AS " + s21::BDNames::bid_id_for_join + ", seller."
                          + std::string(s21::BDNames::user_table_user_name) +
                          " AS " + s21::BDNames::joined_seller_name
                          + ", buyer." + std::string(s21::BDNames::user_table_user_name)
                          + " AS " + s21::BDNames::joined_buyer_name
                          + ", b." + std::string(s21::BDNames::bid_table_rate)
                          + ", b." + std::string(s21::BDNames::bid_table_quantity)
                          + ", b." + std::string(s21::BDNames::bid_table_create_update_time)
                          + " FROM " + std::string(s21::BDNames::bid_table)
                          + " b LEFT JOIN " + std::string(s21::BDNames::user_table) + " seller ON b."
                          + std::string(s21::BDNames::bid_table_seller_id)
                          + " = seller." + std::string(s21::BDNames::user_table_id)
                          + " JOIN "  + std::string(s21::BDNames::user_table) + " buyer ON b."
                          + std::string(s21::BDNames::bid_table_buyer_id)
                          + " = buyer." + std::string(s21::BDNames::user_table_id)
                          + " WHERE buyer." + std::string(s21::BDNames::user_table_id)
                          + " = '" + buyer_id + "'";
        pqxx::result mock_res;
        BOOST_CHECK_NO_THROW({
                                 EXPECT_CALL(mock_db, Execute(sql))
                                         .WillOnce(testing::Return(mock_res));
                                 auto res = bid_repo.ReadAllUserBuyBids(buyer_id);
                                 BOOST_ASSERT(res == mock_res);
                             });
}

BOOST_AUTO_TEST_CASE(DeleteBid) {
        std::string sql = "DELETE FROM " + std::string(s21::BDNames::bid_table)
                          + " WHERE " + std::string(s21::BDNames::bid_table_id)
                          + " = '" + bid_id + "'";
        BOOST_CHECK_NO_THROW({
                                 EXPECT_CALL(mock_db, ExecuteAndCommit(sql))
                                         .WillOnce(testing::Return());
                                 bid_repo.DeleteBid(bid_id);
                             });
}

BOOST_AUTO_TEST_CASE(UpdateBidRate) {
        std::string sql = "UPDATE " + std::string(s21::BDNames::bid_table)
                          + " SET " + std::string(s21::BDNames::bid_table_rate)
                          + " = " + rate + ", time = '" + time
                          + "' WHERE " + std::string(s21::BDNames::bid_table_id)
                          + " = '" + bid_id + "'";
        BOOST_CHECK_NO_THROW({
                                 EXPECT_CALL(mock_db, ExecuteAndCommit(sql))
                                         .WillOnce(testing::Return());
                                 bid_repo.UpdateBidRate(bid_id, rate, time);
                             });
}

BOOST_AUTO_TEST_CASE(UpdateBidQuantity) {
        std::string sql = "UPDATE " + std::string(s21::BDNames::bid_table)
                          + " SET " + std::string(s21::BDNames::bid_table_quantity)
                          + " = " + quantity + ", time = '" + time
                          + "' WHERE " + std::string(s21::BDNames::bid_table_id)
                          + " = '" + bid_id + "'";
        BOOST_CHECK_NO_THROW({
                                 EXPECT_CALL(mock_db, ExecuteAndCommit(sql))
                                         .WillOnce(testing::Return());
                                 bid_repo.UpdateBidQuantity(bid_id, quantity, time);
                             });
}

BOOST_AUTO_TEST_CASE(MatchBuyBids) {
        std::string sql = "SELECT " + std::string(s21::BDNames::bid_table_id)
                          + ", " + std::string(s21::BDNames::bid_table_rate)
                          + ", " + std::string(s21::BDNames::bid_table_buyer_id)
                          + ", " + std::string(s21::BDNames::bid_table_quantity) +
                          " FROM " + std::string(s21::BDNames::bid_table)
                          + " WHERE " + std::string(s21::BDNames::bid_table_seller_id) + " is NULL AND "
                          + std::string(s21::BDNames::bid_table_buyer_id) + " != '" + user_id
                          + "' AND " + std::string(s21::BDNames::bid_table_rate) + " >= " + rate
                          + " ORDER BY " + std::string(s21::BDNames::bid_table_rate) + " ASC";
        pqxx::result mock_res;
        BOOST_CHECK_NO_THROW({
                                 EXPECT_CALL(mock_db, Execute(sql))
                                         .WillOnce(testing::Return(mock_res));
                                 auto res = bid_repo.MatchBuyBids(user_id, rate);
                                 BOOST_ASSERT(res == mock_res);
                             });
}

BOOST_AUTO_TEST_CASE(MatchSellBids) {
        std::string sql = "SELECT " + std::string(s21::BDNames::bid_table_id)
                          + ", " + std::string(s21::BDNames::bid_table_rate)
                          + ", " + std::string(s21::BDNames::bid_table_seller_id)
                          + ", " + std::string(s21::BDNames::bid_table_quantity)
                          + " FROM " + std::string(s21::BDNames::bid_table)
                          + " WHERE " + std::string(s21::BDNames::bid_table_buyer_id) + " is NULL AND "
                          + std::string(s21::BDNames::bid_table_seller_id) + " != '" + user_id
                          + "' AND " + std::string(s21::BDNames::bid_table_rate) + " <= " + rate
                          + " ORDER BY " + std::string(s21::BDNames::bid_table_rate) + " DESC";
        pqxx::result mock_res;
        BOOST_CHECK_NO_THROW({
                                 EXPECT_CALL(mock_db, Execute(sql))
                                         .WillOnce(testing::Return(mock_res));
                                 auto res = bid_repo.MatchSellBids(user_id, rate);
                                 BOOST_ASSERT(res == mock_res);
                             });
}

BOOST_AUTO_TEST_SUITE_END()

