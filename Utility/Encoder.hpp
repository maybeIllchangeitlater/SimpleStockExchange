#ifndef SIMPLESTOCKEXCHANGE_UTILITY_ENCODER_HPP
#define SIMPLESTOCKEXCHANGE_UTILITY_ENCODER_HPP

#include <string>

#include "../3rdParty/cryptopp890/filters.h"
#include "../3rdParty/cryptopp890/hex.h"
#include "../3rdParty/cryptopp890/sha.h"

namespace s21 {
struct Encoder {
  /// Encodes a string into SHA512 hash. Passwords are stored in DB in hashed
  /// version
  static std::string Encode(const std::string& str) {
    if (!str.empty()) {
      CryptoPP::SHA512 hash;
      std::string hashed_password;
      CryptoPP::StringSource(
          str, true,
          new CryptoPP::HashFilter(
              hash, new CryptoPP::HexEncoder(
                        new CryptoPP::StringSink(hashed_password), false)));
      return hashed_password;
    } else {
      return str;
    }
  }
};
}  // namespace s21

#endif  // SIMPLESTOCKEXCHANGE_UTILITY_ENCODER_HPP
