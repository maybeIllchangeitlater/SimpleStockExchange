#include <iostream>
#include "Repository/UserRepository.hpp"
#include "Repository/TransactionRepository.hpp"
#include "Repository/BidRepository.hpp"
#include "Service/UserService.hpp"
#include "Service/BidService.hpp"
#include "Service/TransactionService.hpp"
#include "Controller/UserController.hpp"
#include "Controller/BidController.hpp"
#include "Controller/TransactionController.hpp"
#include "Server/Server.hpp"

int main() {
    pqxx::connection connection("dbname=postgres user=postgres password=postgres host=localhost port=5432");
//    if(connection.is_open()){
//        std::cout << "connected to db\n";
//        pqxx::work task(connection);
//        std::string sql = "SELECT username FROM user_info;";
//        auto result = task.exec(sql);
//        std::cout << "got " << result.size() << "from debug\n";
//        std::cout << "select usernmae res is: " << result[0]["username"].as<std::string>() << "\ntrying to insert";
//        try {
//            std::string sql2 = "INSERT INTO user_info (username, password, balance) VALUES ('aboba', 'biba123', 123);";
//            result = task.exec(sql2);
//            task.commit();
//            std::cout << "inserted\n";
//        }catch (const std::exception &e){
//            std::cout << e.what();
//        }
//    }
    s21::BalanceRepository balance_repository(connection);
    s21::UserRepository user_repository(connection);
    s21::BidRepository bid_repository(connection);
    s21::TransactionRepository transaction_repository(connection);
    s21::BalanceService balance_service(balance_repository);
    s21::UserService user_service(user_repository, balance_service);
    s21::TransactionService transaction_service(transaction_repository, balance_service);
    s21::BidService bid_service(bid_repository, transaction_service);
    s21::UserController user_controller(user_service);
    s21::BidController bid_controller(bid_service);
    s21::TransactionController transaction_controller(transaction_service);
    s21::Server server(5050, user_controller, bid_controller, transaction_controller);
    server.Start(); //add upd
    while(true){
        server.Update();
    }
}

///2 threads for server? prob not needed
///custom errors for deleting finished/unfinished bid
///start with server
///problem with message getting lost is write method in connection that pops messages

