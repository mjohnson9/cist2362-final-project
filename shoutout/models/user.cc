// Copyright 2019 Michael Johnson

#include "shoutout/models/user.h"
#include "shoutout/models/privateshout.h"
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
  this->private_shouts_ = new std::vector<PrivateShout*>();
}

bool User::RemoveFollower(User* user) {
  const std::string to_delete = user->UserId();

  for (auto iterator = this->followers_->begin();
       iterator != this->followers_->end(); iterator++) {
    User* i = *iterator;
    if (i->UserId() == to_delete) {
      this->followers_->erase(iterator);
      return true;
    }
  }

  return false;
}

bool User::RemoveFollowing(User* user) {
  const std::string to_delete = user->UserId();

  for (auto iterator = this->following_->begin();
       iterator != this->following_->end(); iterator++) {
    User* i = *iterator;
    if (i->UserId() == to_delete) {
      this->following_->erase(iterator);
      return true;
    }
  }

  return false;
}

bool User::RemoveShout(Shout* shout) {
  for (auto iterator = this->shouts_->begin(); iterator != this->shouts_->end();
       iterator++) {
    Shout* i = *iterator;
    if (i == shout) {
      this->shouts_->erase(iterator);
      return true;
    }
  }

  return false;
}

bool User::RemovePrivateShout(PrivateShout* shout) {
  for (auto iterator = this->private_shouts_->begin();
       iterator != this->private_shouts_->end(); iterator++) {
    PrivateShout* i = *iterator;
    if (i == shout) {
      this->private_shouts_->erase(iterator);
      return true;
    }
  }

  return false;
}
}  // namespace shoutout
}  // namespace mjohnson
