// Copyright 2019 Michael Johnson

#pragma once

#include <chrono>  // NOLINT(build/c++11)
#include <string>

#include "shoutout/models/user.h"

namespace mjohnson {
namespace shoutout {
class User;  // Forward declaration so that we can have the circular dependency

/**
 * Provides a model of a private shout.
 */
class PrivateShout {
 private:
  /**
   * The user who sent the private shout.
   */
  User* from_;
  /**
   * The user who received the private shout.
   */
  User* to_;
  /**
   * When the private shout was sent.
   */
  std::chrono::system_clock::time_point sent_time_;
  /**
   * The contents of the private shout.
   */
  std::string content_;

 public:
  /**
   * Constructs a new PrivateShout with its time set to now.
   * @param from Who sent the PrivateShout.
   * @param to Who the PrivateShout was sent to.
   * @param content The contents of the PrivateShout.
   */
  PrivateShout(User* from, User* to, std::string content);

  /**
   * Constructs a new PrivateShout with the specified parameters.
   * @param from Who sent the PrivateShout.
   * @param to Who the PrivateShout was sent to.
   * @param content The contents of the PrivateShout.
   * @param sent_time When the PrivateShout was sent.
   */
  PrivateShout(User* from, User* to, std::string content,
               std::chrono::system_clock::time_point sent_time);

  /**
   * Retrieves the user who sent the PrivateShout.
   * @return The user who sent the PrivateShout.
   */
  User* From() const { return this->from_; }

  /**
   * Retrieves the user who received the PrivateShout.
   * @return The user who received the PrivateShout.
   */
  User* To() const { return this->to_; }

  /**
   * Retrieves the time that the PrivateShout was sent.
   * @return The time that the PrivateShout was sent.
   */
  std::chrono::system_clock::time_point Time() const {
    return this->sent_time_;
  }

  /**
   * Retrieves the contents of the PrivateShout.
   * @return The contents of the PrivateShout.
   */
  std::string Content() const { return this->content_; }
};
}  // namespace shoutout
}  // namespace mjohnson
