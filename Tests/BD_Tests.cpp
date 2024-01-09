#define BOOST_TEST_MODULE BD_Test
#include <string>
#include <boost/test/included/unit_test.hpp>
#include <gmock/gmock.h>
#include <boost/asio/io_context.hpp>
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
}
}
