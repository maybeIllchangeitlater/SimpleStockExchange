#include "UserRepository.hpp"

namespace s21 {

    void UserRepository::CreateUser(const std::string &username, const std::string &password,
                                    const std::string &user_balance) {
        pqxx::work task(db_connection_);
        try{

            std::string sql = "INSERT INTO user_info (id, username, password, balance) VALUES ("
                              + task.quote(UUIDGenerator::Generate()) + ", "
                              + task.quote(username) + ", " + task.quote(Encoder::Encode(password)) + ", "
                              + task.quote(user_balance) + ")";
            task.exec(sql);
            task.commit();
        }catch(...){
            task.abort();
            //add err msg?
        }
    }
    void UserRepository::ReadUserById(const std::string &user_id) {
        pqxx::work task(db_connection_);
        try{
            std::string sql = "SELECT id, username, balance FROM user_info WHERE id = " + task.quote(user_id);
            task.exec(sql);
            task.commit();
        }catch(...){
            task.abort();
        }
    }

    pqxx::result UserRepository::ReadUserByName(const std::string &user_name) {
        pqxx::work task(db_connection_);
        pqxx::result res;
        try{
            std::string sql = "SELECT id, username, balance FROM user_info WHERE id = " + task.quote(user_name);
            res = task.exec(sql);
            if(!res.empty()) {
                return res;
            }else{
                throw std::runtime_error("User Not Found");
            }
        }catch(...){
            task.abort();
            throw std::runtime_error("Error Processing Request");
        }
    }


    void UserRepository::UpdateUserName(const std::string &user_id, const std::string &new_username) {
        pqxx::work task(db_connection_);
        try{
            std::string sql = "UPDATE user_info SET username = " + task.quote(new_username) + " WHERE id = "
                    + task.quote(user_id);
            task.exec(sql);
            task.commit();
        }catch(...){
            task.abort();
        }
    }

    void UserRepository::UpdateUserPassword(const std::string &user_id, const std::string &new_password) {
        pqxx::work task(db_connection_);
        try{
            std::string sql = "UPDATE user_info SET password = " + task.quote(Encoder::Encode(new_password)) + " WHERE id = "
                              + task.quote(user_id);
            task.exec(sql);
            task.commit();
        }catch(...){
            task.abort();
        }
    }

    void UserRepository::UpdateUserBalance(const std::string &user_id, const std::string &new_balance) {
        pqxx::work task(db_connection_);
        try{
            std::string sql = "UPDATE user_info SET balance = " + task.quote(new_balance) + " WHERE id = "
                              + task.quote(user_id);
            task.exec(sql);
            task.commit();
        }catch(...){
            task.abort();
        }
    }

    void UserRepository::DeleteUser(const std::string &user_id) {
        pqxx::work task(db_connection_);
        try{
            std::string sql = "DELETE FROM user_info WHERE id = "
                              + task.quote(user_id);
            task.exec(sql);
            task.commit();
        }catch(...){
            task.abort();
        }
    }
} // s21