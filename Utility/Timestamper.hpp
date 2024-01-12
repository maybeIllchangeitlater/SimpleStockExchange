#ifndef SIMPLESTOCKEXCHANGE_UTILITY_TIMESTAMPER_HPP
#define SIMPLESTOCKEXCHANGE_UTILITY_TIMESTAMPER_HPP

#include <boost/date_time/posix_time/posix_time.hpp>
#include <string>

namespace s21 {
struct Timestamper {
  static std::string GetTimestamp() {
    return boost::posix_time::to_iso_extended_string(
        boost::posix_time::microsec_clock::universal_time());
  }
};
}  // namespace s21

#endif  // SIMPLESTOCKEXCHANGE_UTILITY_TIMESTAMPER_HPP
