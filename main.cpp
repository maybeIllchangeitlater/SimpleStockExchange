#include "Controller/BidController.hpp"
#include "Controller/TransactionController.hpp"
#include "Controller/UserController.hpp"
#include "Repository/BidRepository.hpp"
#include "Repository/DatabaseImpl.hpp"
#include "Repository/TransactionRepository.hpp"
#include "Repository/UserRepository.hpp"
#include "Server/Server.hpp"
#include "Service/BidService.hpp"
#include "Service/TransactionService.hpp"
#include "Service/UserService.hpp"

int main() {
  pqxx::connection connection(
      "dbname=postgres user=postgres password=postgres host=postgres-container "
      "port=5432");
  s21::DatabaseImpl db(connection);
  s21::BalanceRepository balance_repository(db);
  s21::UserRepository user_repository(db);
  s21::BidRepository bid_repository(db);
  s21::TransactionRepository transaction_repository(db);
  s21::BalanceService balance_service(balance_repository);
  s21::UserService user_service(user_repository, balance_service);
  s21::TransactionService transaction_service(transaction_repository,
                                              balance_service);
  s21::BidService bid_service(bid_repository, transaction_service);
  s21::UserController user_controller(user_service);
  s21::BidController bid_controller(bid_service);
  s21::TransactionController transaction_controller(transaction_service);
  s21::Server server(5050, user_controller, bid_controller,
                     transaction_controller);
  server.Start();  // add upd
  while (true) {
    server.Update();
  }
}

/// 2 threads for server? prob not needed
/// start with server
