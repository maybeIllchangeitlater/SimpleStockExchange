#ifndef SIMPLESTOCKEXCHANGE_REPOSITORY_DATABASEINTERFACE_HPP
#define SIMPLESTOCKEXCHANGE_REPOSITORY_DATABASEINTERFACE_HPP

#include <string>
#include "../3rdParty/libpqxx/include/pqxx/pqxx"

namespace s21 { //for sake of mock testing
    class DatabaseInterface {
    public:
        virtual ~DatabaseInterface() = default;
        ///executes sql query and return execution result
        virtual pqxx::result Execute(const std::string &sql) = 0;
        ///execute and commit sql query
        virtual void ExecuteAndCommit(const std::string &sql) = 0;
    };
} // s21

#endif //SIMPLESTOCKEXCHANGE_REPOSITORY_DATABASEINTERFACE_HPP
