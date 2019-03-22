// Copyright 2019 Michael Johnson

#pragma once

#include <string>

#include "shoutout/models/user.h"
#include "shoutout/views/view.h"

namespace mjohnson {
namespace shoutout {
/**
 * Provides a screen for user registration. This allows a user to
 */
class ShoutView : public View {
 private:
  User* user_;

  static bool ValidateShout(const std::string& shout);

 public:
  explicit ShoutView(User* user) : user_(user) {
    if (user == nullptr) {
      throw std::invalid_argument("user cannot be null");
    }
  }

  View* Display() override;
};
}  // namespace shoutout
}  // namespace mjohnson
