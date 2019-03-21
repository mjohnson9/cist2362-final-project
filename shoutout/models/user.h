// Copyright 2019 Michael Johnson

#pragma once

#include <string>
#include <utility>
#include <vector>

namespace mjohnson {
namespace shoutout {
/**
 * Represents a user.
 */
class User {
 private:
  std::string username_;
  std::string password_;

  std::vector<User*>* following_;
  std::vector<User*>* followers_;

  void Initialize();

 public:
  static std::string CreateUserId(const std::string& username);

  explicit User(std::string username);
  User(std::string username, std::string password);

  /**
   * Retrieves the User's username.
   * @return The username.
   */
  std::string Username() const { return this->username_; }

  /**
   * Builds the user ID. The user ID is the username in all lowercase with
   * special characters removed.
   * @return [description]
   */
  std::string UserId() const { return User::CreateUserId(this->username_); }

  std::vector<User*>* Following() { return this->following_; }
  void AddFollowing(User* user);

  std::vector<User*>* Followers() { return this->followers_; }
  void AddFollower(User* user);
};
}  // namespace shoutout
}  // namespace mjohnson
