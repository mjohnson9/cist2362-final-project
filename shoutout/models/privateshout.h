// Copyright 2019 Michael Johnson

#pragma once

#include <chrono>  // NOLINT(build/c++11)
#include <string>

#include "shoutout/models/user.h"

namespace mjohnson {
namespace shoutout {
class User;  // Forward declaration so that we can have the circular dependency

class PrivateShout {
 private:
  User* from_;
  User* to_;
  std::chrono::system_clock::time_point sent_time_;
  std::string content_;

 public:
  PrivateShout(User* from, User* to, std::string content);
  PrivateShout(User* from, User* to, std::string content,
               std::chrono::system_clock::time_point sent_time);

  User* From() const { return this->from_; }

  User* To() const { return this->to_; }

  std::chrono::system_clock::time_point Time() const {
    return this->sent_time_;
  }

  std::string Content() const { return this->content_; }
};
}  // namespace shoutout
}  // namespace mjohnson
