// Copyright 2019 Michael Johnson

#pragma once

#include <string>

#include "shoutout/views/view.h"

namespace mjohnson {
namespace shoutout {
class LoginView : public View {
 private:
  static bool ValidateUsername(const std::string& username);
  static bool ValidatePassword(const std::string& password);

 public:
  LoginView() = default;

  View* Display() override;
};
}  // namespace shoutout
}  // namespace mjohnson
