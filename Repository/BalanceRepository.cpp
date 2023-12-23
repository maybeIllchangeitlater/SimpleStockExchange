#include "BalanceRepository.hpp"

namespace s21{
    void BalanceRepository::CreateUserBalance(const std::string &user_id, const std::string &balance_usd,
                                              const std::string &balance_rub) {
        pqxx::work task(db_connection_);
        pqxx::result res;
        try{
            std::string sql = "INSERT INTO " + std::string(BDNames::balance_table)
                    + " (" + BDNames::balance_table_user_id
                    + ", " + BDNames::balance_table_usd
                    + ", " + BDNames::balance_table_rub
                    + ") VALUES (" + task.quote(user_id)
                    + ", " + balance_usd
                    + ", " + balance_rub + ");";
            std::cout << sql << "\n";
            task.exec(sql);
            task.commit();
        }catch(const std::exception &e){
            std::cout << "\naboorting task because " << e.what() << "\n";
            task.abort();
            throw;
        }
    }
    pqxx::result BalanceRepository::GetUserBalance(const std::string &user_id) {
        pqxx::work task(db_connection_);
        pqxx::result res;
        try{
            std::string sql = "SELECT " + std::string(BDNames::balance_table_user_id)
                              + ", " + BDNames::balance_table_usd
                              + ", " + BDNames::balance_table_rub
                              + " FROM " + BDNames::balance_table
                              + " WHERE " + BDNames::balance_table_user_id
                              + " = " + task.quote(user_id);
            std::cout << sql << "\n";
            auto result = task.exec(sql);
            if(result.empty()){
                throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::USER_NOT_FOUND));
            }
            return result;
        }catch(const std::exception &e){
            std::cout << "\naboorting task because " << e.what() << "\n";
            task.abort();
            throw;
        }
    }

    void BalanceRepository::SetUserBalance(const std::string &user_id, const std::string &balance_usd,
                                           const std::string &balance_rub) {
        pqxx::work task(db_connection_);
        try{
            std::string sql = "UPDATE " + std::string(BDNames::balance_table)
                              + " SET " + BDNames::balance_table_usd
                              + " = " + task.quote(balance_usd) + ", " + BDNames::balance_table_rub
                              + " = " + task.quote(balance_rub)
                              + " WHERE " + std::string(BDNames::balance_table_user_id)
                              + " = " + task.quote(user_id);
            std::cout << sql << "\n";
            task.exec(sql);
            task.commit();
        }catch(const std::exception &e){
            std::cout << "\naboorting task because " << e.what() << "\n";
            task.abort();
            throw;
        }
    }
}