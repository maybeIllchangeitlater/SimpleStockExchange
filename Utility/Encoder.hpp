#ifndef SIMPLESTOCKEXCHANGE_ENCODER_HPP_
#define SIMPLESTOCKEXCHANGE_ENCODER_HPP_

#include <string>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>

namespace s21 {
    struct Encoder {
        static std::string Encode(const std::string& str){
            if(!str.empty()) {
                CryptoPP::SHA512 hash;
                std::string hashed_password;
                CryptoPP::StringSource(hashed_password, true, new CryptoPP::HashFilter(hash, new CryptoPP::HexEncoder(
                        new CryptoPP::StringSink(hashed_password), false)));
                return hashed_password;
            }else{
                return str;
            }
        }
    };
} //s21

#endif //SIMPLESTOCKEXCHANGE_ENCODER_HPP_
