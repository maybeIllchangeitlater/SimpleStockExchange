#include "UserRepository.hpp"

namespace s21 {

    void UserRepository::CreateUser(const std::string &user_id, const std::string &username,
                                    const std::string &password, const std::string &user_balance) {
        pqxx::work task(db_connection_);
        try{

            std::string sql = "INSERT INTO user_info (id, username, password, balance) VALUES ("
                              + task.quote(UUIDGenerator::Generate()) + ", "
                              + task.quote(username) + ", " + task.quote(password) + ", "
                              + task.quote(user_balance) + ")";
            if(!task.exec(sql).empty()) {
                task.commit();
            }
            else{
                throw std::runtime_error("Error Creating User");
            }
        }catch(...){
            task.abort();
            throw std::runtime_error("Error Creating User");
        }
    }
    pqxx::result UserRepository::ReadUserById(const std::string &user_id) {
        pqxx::work task(db_connection_);
        pqxx::result res;
        try{
            std::string sql = "SELECT id, username, password, balance FROM user_info WHERE id = " + task.quote(user_id);
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

    pqxx::result UserRepository::ReadUserByName(const std::string &user_name) {
        pqxx::work task(db_connection_);
        pqxx::result res;
        try{
            std::string sql = "SELECT id, username, balance, password FROM user_info WHERE user_name = " + task.quote(user_name);
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
            if(!task.exec(sql).empty()) {
                task.commit();
            }else{
                throw std::logic_error("Error Updating User Name");
            }
        }catch(...){
            task.abort();
            throw std::logic_error("Error Updating User Name");
        }
    }

    void UserRepository::UpdateUserPassword(const std::string &user_id, const std::string &new_password) {
        pqxx::work task(db_connection_);
        try{
            std::string sql = "UPDATE user_info SET password = " + task.quote(new_password) + " WHERE id = "
                              + task.quote(user_id);
            if(!task.exec(sql).empty()) {
                task.commit();
            }else{
                throw std::logic_error("Error Updating User Password");
            }
        }catch(...){
            task.abort();
            throw std::logic_error("Error Updating User Password");
        }
    }

    void UserRepository::UpdateUserBalance(const std::string &user_id, const std::string &new_balance) {
        pqxx::work task(db_connection_);
        try{
            std::string sql = "UPDATE user_info SET balance = " + task.quote(new_balance) + " WHERE id = "
                              + task.quote(user_id);
            if(!task.exec(sql).empty()) {
                task.commit();
            }else{
                throw std::logic_error("Error Updating User Balance");
            }
        }catch(...){
            task.abort();
            throw std::logic_error("Error Updating User Balance");
        }
    }

    void UserRepository::DeleteUser(const std::string &user_id) {
        pqxx::work task(db_connection_);
        try {
            std::string sql = "DELETE FROM user_info WHERE id = "
                              + task.quote(user_id);
            if (!task.exec(sql).empty()) {
                task.commit();
            } else {
                throw std::logic_error("User Not Found");
            }
        } catch (...) {
            task.abort();
            throw std::logic_error("Error Deleting User");
        }
    }
} // s21