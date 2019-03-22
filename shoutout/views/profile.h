// Copyright 2019 Michael Johnson

#pragma once

#include <stdexcept>
#include <vector>

#include "shoutout/models/user.h"
#include "shoutout/views/view.h"

namespace mjohnson {
namespace shoutout {
class ProfileView : public View {
 private:
  User* logged_in_user_;
  User* viewing_user_;

  void PrintShoutList(std::vector<Shout*>* shouts);

 public:
  ProfileView(User* logged_in, User* viewing)
      : logged_in_user_(logged_in), viewing_user_(viewing) {
    if (logged_in == nullptr) {
      throw std::invalid_argument("logged_in cannot be null");
    }
    if (viewing == nullptr) {
      throw std::invalid_argument("viewing cannot be null");
    }
  }

  View* Display() override;
};
}  // namespace shoutout
}  // namespace mjohnson
