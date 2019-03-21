// Copyright 2019 Michael Johnson

#pragma once

#include "shoutout/models/user.h"
#include "shoutout/views/view.h"

namespace mjohnson {
namespace shoutout {
/**
 * Provides the main menu. The main menu allows a user to register or login. It
 * can also display an error message from another view.
 */
class HomeView : public View {
 private:
  /**
   * The error message to be displayed with this main menu view.
   */
  User* user_;

 public:
  explicit HomeView(User* user) : user_(user) {}

  View* Display() override;
};
}  // namespace shoutout
}  // namespace mjohnson
