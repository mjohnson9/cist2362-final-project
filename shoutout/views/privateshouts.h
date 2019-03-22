// Copyright 2019 Michael Johnson

#pragma once

#include <stdexcept>
#include <string>

#include "shoutout/models/user.h"
#include "shoutout/views/view.h"

namespace mjohnson {
namespace shoutout {
class PrivateShoutsView : public View {
 private:
  User* user_;
  User* constructing_shout_to_;

  void DisplayConstructShout();
  void DisplayDelete();

 public:
  explicit PrivateShoutsView(User* user)
      : user_(user), constructing_shout_to_(nullptr) {
    if (user == nullptr) {
      throw std::invalid_argument("user cannot be null");
    }
  }

  PrivateShoutsView(User* user, User* shout_to)
      : user_(user), constructing_shout_to_(shout_to) {
    if (user == nullptr) {
      throw std::invalid_argument("user cannot be null");
    }
    if (shout_to == nullptr) {
      throw std::invalid_argument("shout_to cannot be null");
    }
  }

  View* Display() override;
};
}  // namespace shoutout
}  // namespace mjohnson
