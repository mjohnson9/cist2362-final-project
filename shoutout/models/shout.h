// Copyright 2019 Michael Johnson

#pragma once

#include <chrono>  // NOLINT(build/c++11)
#include <stdexcept>
#include <string>
#include <utility>

namespace mjohnson {
namespace shoutout {
class User;  // Forward declaration of User because of circular dependency

/**
 * Provides the main menu. The main menu allows a user to register or login. It
 * can also display an error message from another view.
 */
class Shout {
 private:
  User* owner_;
  std::string content_;
  std::chrono::system_clock::time_point posted_time_;

 public:
  Shout(User* owner, std::string content)
      : owner_(owner), content_(std::move(content)) {
    if (owner == nullptr) {
      throw std::invalid_argument("owner cannot be null");
    }

    this->posted_time_ = std::chrono::system_clock::now();
  }

  Shout(User* owner, std::string content,
        std::chrono::system_clock::time_point posted_time)
      : owner_(owner), content_(std::move(content)), posted_time_(posted_time) {
    if (owner == nullptr) {
      throw std::invalid_argument("owner cannot be null");
    }
  }

  User* Owner() const { return this->owner_; }

  std::string Content() const { return this->content_; }

  std::chrono::system_clock::time_point Time() const {
    return this->posted_time_;
  }
};

bool ShoutTimeCompare(Shout* shout1, Shout* shout2);
}  // namespace shoutout
}  // namespace mjohnson
