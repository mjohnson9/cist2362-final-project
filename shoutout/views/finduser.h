// Copyright 2019 Michael Johnson

#pragma once

#include <stdexcept>
#include <string>

#include "shoutout/models/user.h"
#include "shoutout/views/view.h"

namespace mjohnson {
namespace shoutout {
class FindUserView : public View {
 private:
  User* user_;

  static bool ValidateSearch(const std::string& search);

 public:
  explicit FindUserView(User* user) : user_(user) {
    if (user == nullptr) {
      throw std::invalid_argument("user cannot be null");
    }
  }

  View* Display() override;
};
}  // namespace shoutout
}  // namespace mjohnson
