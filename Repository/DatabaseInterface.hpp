#ifndef SIMPLESTOCKEXCHANGE_REPOSITORY_DATABASEINTERFACE_HPP
#define SIMPLESTOCKEXCHANGE_REPOSITORY_DATABASEINTERFACE_HPP

#include <string>
#include "../3rdParty/libpqxx/include/pqxx/pqxx"

namespace s21 { //for sake of mock testing
    class DatabaseInterface {
    public:
        virtual ~DatabaseInterface() = default;
        virtual pqxx::result Execute(const std::string &sql) = 0;
        virtual void ExecuteAndCommit(const std::string &sql) = 0;
    };
} // s21

#endif //SIMPLESTOCKEXCHANGE_REPOSITORY_DATABASEINTERFACE_HPP
