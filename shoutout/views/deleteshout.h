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
class DeleteShoutView : public View {
 private:
  User* user_;

 public:
  explicit DeleteShoutView(User* user) : user_(user) {
    if (user == nullptr) {
      throw std::invalid_argument("user cannot be null");
    }
  }

  View* Display() override;
};
}  // namespace shoutout
}  // namespace mjohnson
