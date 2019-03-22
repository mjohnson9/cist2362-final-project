// Copyright 2019 Michael Johnson

#include "shoutout/models/privateshout.h"

#include <string>
#include <utility>

namespace mjohnson {
namespace shoutout {

PrivateShout::PrivateShout(User* from, User* to, std::string content)
    : from_(from), to_(to), content_(std::move(content)) {
  if (from == nullptr) {
    throw std::invalid_argument("from cannot be null");
  }
  if (to == nullptr) {
    throw std::invalid_argument("to cannot be null");
  }
  this->sent_time_ = std::chrono::system_clock::now();
}

PrivateShout::PrivateShout(User* from, User* to, std::string content,
                           std::chrono::system_clock::time_point sent_time)
    : from_(from),
      to_(to),
      sent_time_(sent_time),
      content_(std::move(content)) {
  if (from == nullptr) {
    throw std::invalid_argument("from cannot be null");
  }
  if (to == nullptr) {
    throw std::invalid_argument("to cannot be null");
  }
}
}  // namespace shoutout
}  // namespace mjohnson
