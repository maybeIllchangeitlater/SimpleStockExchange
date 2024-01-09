#ifndef SIMPLESTOCKEXCHANGE_REPOSITORY_DATABASEIMPL_HPP
#define SIMPLESTOCKEXCHANGE_REPOSITORY_DATABASEIMPL_HPP

#include "DatabaseInterface.hpp"

namespace s21 {
    class DatabaseImpl : public DatabaseInterface{
    public:

        explicit DatabaseImpl(pqxx::connection &connection) : connection_(connection){}
        pqxx::result Execute(const std::string &sql) override{
            pqxx::work task(connection_);
            try {
                auto result = task.exec(sql);
                return result;
            }catch(const std::exception &e){
                task.abort();
                throw;
            }
        }
        void ExecuteAndCommit(const std::string &sql) override{
            pqxx::work task(connection_);
            try {
                task.exec(sql);
                task.commit();
            }catch(const std::exception &e){
                task.abort();
                throw;
            }
        }
    private:
        pqxx::connection &connection_;
    };
}

#endif //SIMPLESTOCKEXCHANGE_REPOSITORY_DATABASEIMPL_HPP
