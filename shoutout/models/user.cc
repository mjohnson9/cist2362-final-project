// Copyright 2019 Michael Johnson

#include "shoutout/models/user.h"
#include "shoutout/models/shout.h"

#include "shoutout/common.h"

namespace mjohnson {
namespace shoutout {
std::string User::CreateUserId(const std::string& username) {
  std::string username_copy(username);
  mjohnson::common::LowerString(&username_copy);
  return username_copy;
}

User::User(std::string username)
    : username_(std::move(username)), password_("") {
  this->Initialize();
}
User::User(std::string username, std::string password)
    : username_(std::move(username)), password_(std::move(password)) {
  this->Initialize();
}

void User::Initialize() {
  this->following_ = new std::vector<User*>();
  this->followers_ = new std::vector<User*>();
  this->shouts_ = new std::vector<Shout*>();
}

void User::AddFollower(User* user) { this->followers_->push_back(user); }

void User::AddFollowing(User* user) { this->following_->push_back(user); }
}  // namespace shoutout
}  // namespace mjohnson
