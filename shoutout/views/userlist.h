// Copyright 2019 Michael Johnson

#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include "shoutout/models/user.h"
#include "shoutout/views/view.h"

namespace mjohnson {
namespace shoutout {
class UserListView : public View {
 public:
  // We have to have two public sections so that we can keep the list type as a
  // private variable, while the enum is public
  enum UserListType { kFollowersList, kFollowingList };

 private:
  User* logged_in_user_;
  User* viewing_user_;
  UserListType list_type_;

  User* PromptUserSelection(std::vector<User*>* users,
                            const std::string& prompt);
  void DisplayDelete();
  View* DisplaySearch();
  void DisplayHeader();

 public:
  explicit UserListView(User* logged_in, User* viewing, UserListType list_type)
      : logged_in_user_(logged_in),
        viewing_user_(viewing),
        list_type_(list_type) {
    if (logged_in == nullptr) {
      throw std::invalid_argument("user cannot be null");
    }
    if (viewing == nullptr) {
      throw std::invalid_argument("viewing cannot be null");
    }
  }

  View* Display() override;
};
}  // namespace shoutout
}  // namespace mjohnson
