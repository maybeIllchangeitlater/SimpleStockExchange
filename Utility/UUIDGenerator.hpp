#ifndef SIMPLESTOCKEXCHANGE_UUIDGENERATOR_HPP
#define SIMPLESTOCKEXCHANGE_UUIDGENERATOR_HPP

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <string>
namespace s21{
    struct UUIDGenerator{
        static std::string Generate(){
            boost::uuids::random_generator generator;
            boost::uuids::uuid uuid = generator();
            return boost::uuids::to_string(uuid);
        }
    };
} //s21
#endif //SIMPLESTOCKEXCHANGE_UUIDGENERATOR_HPP
