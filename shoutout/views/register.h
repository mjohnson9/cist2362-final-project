// Copyright 2019 Michael Johnson

#pragma once

#include <string>

#include "shoutout/views/view.h"

namespace mjohnson {
namespace shoutout {
/**
 * Provides a screen for user registration. This allows a user to
 */
class RegisterView : public View {
 private:
  std::string username_;
  bool password_mismatch_ = false;

 public:
  RegisterView() : username_("") {}

  View* Display() override;
};
}  // namespace shoutout
}  // namespace mjohnson
