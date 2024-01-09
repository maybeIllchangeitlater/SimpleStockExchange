#include "UserRepository.hpp"

namespace s21 {

    void UserRepository::CreateUser(const std::string &username,
                                    const std::string &password) {
            std::string sql = "INSERT INTO " +  std::string(BDNames::user_table) +
                    "(" + BDNames::user_table_user_name + ", "
                    + BDNames::user_table_password
                    + ") VALUES ('" +
                     username + "', '" + password + "');";
            db_.ExecuteAndCommit(sql);
    }

    pqxx::result UserRepository::ReadUserById(const std::string &user_id) {
            std::string sql = "SELECT " + std::string(BDNames::user_table_id)
                    + ", " + BDNames::user_table_user_name
                    + ", " + BDNames::user_table_password
                    + " FROM " + BDNames::user_table
                    + " WHERE " + BDNames::user_table_id
                    + " = '" + user_id +"'";
            return db_.Execute(sql);
    }

    pqxx::result UserRepository::ReadUserByName(const std::string &username) {
            std::string sql = "SELECT " + std::string(BDNames::user_table_id)
                    + ", " + BDNames::user_table_user_name
                    + ", " + BDNames::user_table_password
                    + " FROM " + BDNames::user_table
                    + " WHERE " + BDNames::user_table_user_name
                    + " = '" + username + "'";
            auto result = db_.Execute(sql);
            if(result.empty()){
                throw std::runtime_error(ServerMessage::server_message.at(ServerMessage::USER_NOT_FOUND));
            }
            return result;
    }


    void UserRepository::UpdateUserName(const std::string &user_id, const std::string &new_username) {
            std::string sql = "UPDATE " + std::string(BDNames::user_table)
                    + " SET " + BDNames::user_table_user_name
                    + " = '" + new_username
                    + "' WHERE " + BDNames::user_table_id
                    + " = '" + user_id + "'";
            db_.ExecuteAndCommit(sql);
    }

    void UserRepository::UpdateUserPassword(const std::string &user_id, const std::string &new_password) {
            std::string sql = "UPDATE " + std::string(BDNames::user_table)
                    + " SET " + BDNames::user_table_password
                    + " = '" + new_password
                    + "' WHERE " + BDNames::user_table_id
                    + " = '" + user_id + "'";
            db_.ExecuteAndCommit(sql);
    }


    void UserRepository::DeleteUser(const std::string &user_id) {
            std::string sql = "DELETE FROM " + std::string(BDNames::user_table)
                    + " WHERE " + BDNames::user_table_id
                    + " = '" + user_id + "'";
            db_.ExecuteAndCommit(sql);
    }
} // s21