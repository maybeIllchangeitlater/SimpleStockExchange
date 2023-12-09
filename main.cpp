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
#include <boost/thread.hpp>
#include <thread>
#include <memory>

int main() {
    pqxx::connection connection("dbname=postgres user=postgres password=postgres host=localhost port=5432");
    s21::UserRepository user_repository(connection);
    s21::BidRepository bid_repository(connection);
    s21::TransactionRepository transaction_repository(connection);
    s21::UserService user_service(user_repository);
    s21::BidService bid_service(bid_repository);
    s21::TransactionService transaction_service(transaction_repository);
    s21::UserController user_controller(user_service);
    s21::BidController bid_controller(bid_service);
    s21::TransactionController transaction_controller(transaction_service);
    s21::Server server(5050, user_controller, bid_controller, transaction_controller);
    server.Start(); //add upd
    while(true){
        server.Update(-1, true);
    }


}

///2 threads for server? prob not needed
///custom errors for deleting finished/unfinished bid
///start with server
