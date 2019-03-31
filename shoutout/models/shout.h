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
 * Provides a model of a single shout.
 */
class Shout {
 private:
  /**
   * The user who sent the shout.
   */
  User* owner_;
  /**
   * The content of the shout.
   */
  std::string content_;
  /**
   * When the shout was posted.
   */
  std::chrono::system_clock::time_point posted_time_;

 public:
  /**
   * Constructs a new Shout. The time of the shout is defaulted to now.
   * @param owner The user who created the shout.
   * @param content The text content of the shout.
   */
  Shout(User* owner, std::string content)
      : owner_(owner), content_(std::move(content)) {
    if (owner == nullptr) {
      throw std::invalid_argument("owner cannot be null");
    }

    this->posted_time_ = std::chrono::system_clock::now();
  }

  /**
   * Constructs a new Shout.
   * @param owner The user who created the shout.
   * @param content The text content of the shout.
   * @param posted_time When the shout was posted.
   */
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

  static bool TimeCompare(Shout* shout1, Shout* shout2);
};
}  // namespace shoutout
}  // namespace mjohnson
