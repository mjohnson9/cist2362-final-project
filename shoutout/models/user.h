// Copyright 2019 Michael Johnson

#pragma once

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "shoutout/models/privateshout.h"
#include "shoutout/models/shout.h"

namespace mjohnson {
namespace shoutout {
// Forward declaration so that we can have the circular dependency
class PrivateShout;

/**
 * A model to represent an individual user.
 */
class User {
 private:
  std::string username_;
  std::string password_;

  std::vector<User*>* following_;
  std::vector<User*>* followers_;

  std::vector<Shout*>* shouts_;
  std::vector<PrivateShout*>* private_shouts_;

  void Initialize();

 public:
  static std::string CreateUserId(const std::string& username);
  static bool UsernameCompare(User* user1, User* user2) {
    return user1->Username() < user2->Username();
  }

  explicit User(std::string username);
  User(std::string username, std::string password);

  /**
   * Retrieves the User's username.
   * @return The username.
   */
  std::string Username() const { return this->username_; }

  std::string Password() const { return this->password_; }

  /**
   * Builds the user ID. The user ID is the username in all lowercase with
   * special characters removed.
   * @return [description]
   */
  std::string UserId() const { return User::CreateUserId(this->username_); }

  std::vector<User*>* Following() const { return this->following_; }
  void AddFollowing(User* user) { this->following_->push_back(user); }
  bool RemoveFollowing(User* user);

  std::vector<User*>* Followers() const { return this->followers_; }
  void AddFollower(User* user) { this->followers_->push_back(user); }
  bool RemoveFollower(User* user);

  std::vector<Shout*>* Shouts() const { return this->shouts_; }
  void AddShout(Shout* shout) {
    this->shouts_->push_back(shout);
    std::sort(this->shouts_->begin(), this->shouts_->end(), Shout::TimeCompare);
  }
  bool RemoveShout(Shout* shout);

  std::vector<PrivateShout*>* PrivateShouts() const {
    return this->private_shouts_;
  }
  void AddPrivateShout(PrivateShout* shout) {
    this->private_shouts_->push_back(shout);
  }
  bool RemovePrivateShout(PrivateShout* shout);
};
}  // namespace shoutout
}  // namespace mjohnson
