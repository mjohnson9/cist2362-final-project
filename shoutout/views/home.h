// Copyright 2019 Michael Johnson

#pragma once

#include <string>

#include "shoutout/models/user.h"
#include "shoutout/views/view.h"

namespace mjohnson {
namespace shoutout {
class HomeView : public View {
 private:
  User* user_;

  std::string message_;

 public:
  explicit HomeView(User* user) : user_(user), message_("") {}

  View* Display() override;
};
}  // namespace shoutout
}  // namespace mjohnson
