// Copyright 2019 Michael Johnson

#pragma once

#include <string>

#include "shoutout/views/view.h"

namespace mjohnson {
namespace shoutout {
class LoginView : public View {
 public:
  LoginView() = default;

  View* Display() override;
};
}  // namespace shoutout
}  // namespace mjohnson
