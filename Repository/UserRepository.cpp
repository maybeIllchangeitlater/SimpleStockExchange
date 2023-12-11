#include "UserRepository.hpp"
#include <iostream>

namespace s21 {

    void UserRepository::CreateUser(const std::string &user_id, const std::string &username,
                                    const std::string &password, const std::string &user_balance) {
        pqxx::work task(db_connection_);
        try{

            std::string sql = "INSERT INTO " + std::string(BDNames::user_table)
                    + " ("+ std::string(BDNames::user_table_id)
                    + ", " + std::string(BDNames::user_table_user_name)
                    + ", " + std::string(BDNames::user_table_password)
                    + ", " + std::string(BDNames::user_table_balance)
                    + ") VALUES ("
                    + task.quote(user_id) + ", "
                    + task.quote(username) + ", " + task.quote(password) + ", "
                    + user_balance + ");";
            std::cout << sql << "\n";
            auto result = task.exec(sql);
            std::cout << "task.exec username is " << result[0]["username"].as<std::string>();
            task.commit();
        }catch(const std::exception &e){
            std::cout << "aboorting task because " << e.what() << "\n";
            task.abort();
            throw;
        }
    }
    pqxx::result UserRepository::ReadUserById(const std::string &user_id) {
        pqxx::work task(db_connection_);
        pqxx::result res;
        try{
            std::string sql = "SELECT " + task.quote(BDNames::user_table_id)
                    + ", " + task.quote(BDNames::user_table_user_name)
                    + ", " + task.quote(BDNames::user_table_password)
                    + ", " + task.quote(BDNames::user_table_balance)
                    + " FROM " + task.quote(BDNames::user_table)
                    + " WHERE " + task.quote(BDNames::user_table_id)
                    + " = " + task.quote(user_id);
            res = task.exec(sql);
            if(!res.empty()) {
                return res;
            }else{
                throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::USER_NOT_FOUND));
            }
        }catch(...){
            task.abort();
            throw;
        }
    }

    pqxx::result UserRepository::ReadUserByName(const std::string &user_name) {
        pqxx::work task(db_connection_);
        pqxx::result res;
        try{
            std::string sql = "SELECT " + std::string(BDNames::user_table_id)
                    + ", " + std::string(BDNames::user_table_user_name)
                    + ", " + std::string(BDNames::user_table_password)
                    + ", " + std::string(BDNames::user_table_balance)
                    + " FROM " + std::string(BDNames::user_table)
                    + " WHERE " + std::string(BDNames::user_table_user_name)
                    + " = " + task.quote(user_name);
            res = task.exec(sql);
            if(!res.empty()) {
                return res;
            }else{
                throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::USER_NOT_FOUND));
            }
        }catch(...){
            task.abort();
            throw;
        }
    }


    void UserRepository::UpdateUserName(const std::string &user_id, const std::string &new_username) {
        pqxx::work task(db_connection_);
        try{
            std::string sql = "UPDATE " + task.quote(BDNames::user_table_user_name)
                    + " SET " + task.quote(BDNames::user_table_user_name)
                    + " = " + task.quote(new_username)
                    + " WHERE " + task.quote(BDNames::user_table_id)
                    + " = " + task.quote(user_id);
            if(!task.exec(sql).empty()) {
                task.commit();
            }else{
                throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::ERROR));
            }
        }catch(...){
            task.abort();
            throw;
        }
    }

    void UserRepository::UpdateUserPassword(const std::string &user_id, const std::string &new_password) {
        pqxx::work task(db_connection_);
        try{
            std::string sql = "UPDATE " + task.quote(BDNames::user_table_user_name)
                    + " SET " + task.quote(BDNames::user_table_password)
                    + " = " + task.quote(new_password)
                    + " WHERE " + task.quote(BDNames::user_table_id)
                    + " = " + task.quote(user_id);
            if(!task.exec(sql).empty()) {
                task.commit();
            }else{
                throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::ERROR));
            }
        }catch(...){
            task.abort();
            throw;
        }
    }

    void UserRepository::UpdateUserBalance(const std::string &user_id, const std::string &new_balance) {
        pqxx::work task(db_connection_);
        try{
            std::string sql = "UPDATE " + task.quote(BDNames::user_table_user_name)
                    + " SET " + task.quote(BDNames::user_table_balance)
                    + " = " + task.quote(new_balance)
                    + " WHERE " + task.quote(BDNames::user_table_id)
                    + " = " + task.quote(user_id);
            if(!task.exec(sql).empty()) {
                task.commit();
            }else{
                throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::ERROR));
            }
        }catch(...){
            task.abort();
            throw;
        }
    }

    void UserRepository::DeleteUser(const std::string &user_id) {
        pqxx::work task(db_connection_);
        try {
            std::string sql = "DELETE FROM " + task.quote(BDNames::user_table)
                    + " WHERE " + task.quote(BDNames::user_table_id)
                    + " = " + task.quote(user_id);
            if (!task.exec(sql).empty()) {
                task.commit();
            } else {
                throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::ERROR));
            }
        } catch (...) {
            task.abort();
            throw;
        }
    }
} // s21