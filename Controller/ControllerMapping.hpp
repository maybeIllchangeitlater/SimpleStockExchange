#ifndef SIMPLESTOCKEXCHANGE_CONTROLLERMAPPING_HPP
#define SIMPLESTOCKEXCHANGE_CONTROLLERMAPPING_HPP

#include <unordered_map>
#include <functional>
#include <any>
#include "../3rdParty/json.hpp"
#include "UserController.hpp"
#include "BidController.hpp"
#include "TransactionController.hpp"
#include "UserController.hpp"

namespace s21 {
    struct ControllerMapping {
    public:
        static const std::unordered_map<std::string, std::any> method_mapping_user;
        static const std::unordered_map<std::string, std::any> method_mapping_bid;
        static const std::unordered_map<std::string, std::any> method_mapping_transaction;
        static const std::unordered_map<char, std::unordered_map<std::string, std::any>> method_mapping;
    };

}

#endif //SIMPLESTOCKEXCHANGE_CONTROLLERMAPPING_HPP
