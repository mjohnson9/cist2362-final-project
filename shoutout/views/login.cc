// Copyright 2019 Michael Johnson

#include "shoutout/views/login.h"

#include "shoutout/common.h"
#include "shoutout/models/shoutout.h"
#include "shoutout/models/user.h"
#include "shoutout/screen.h"
#include "shoutout/views/home.h"
#include "shoutout/views/mainmenu.h"

namespace mjohnson {
namespace shoutout {
View* LoginView::Display() {
  Screen* screen = Screen::Get();
  ShoutOut* db = ShoutOut::Get();

  screen->Clear();

  std::cout << "========== LOGIN ==========" << std::endl << std::endl;

  auto username = mjohnson::common::RequestInput<std::string>(
      "Username: ", LoginView::ValidateUsername);

  auto password = mjohnson::common::RequestInput<std::string>(
      "Password: ", LoginView::ValidatePassword);

  User* user = db->GetUserByName(username);
  if (user == nullptr || password != user->Password()) {
    return new MainMenuView(
        "ERROR: The provided username or password is incorrect.");
  }

  return new HomeView(user);
}

bool LoginView::ValidateUsername(const std::string& username) {
  std::string username_copy(username);
  mjohnson::common::TrimString(&username_copy);
  if (username_copy.empty()) {
    std::cout << std::endl
              << "ERROR: Your username cannot be blank." << std::endl
              << std::endl;

    return false;
  }

  return true;
}

bool LoginView::ValidatePassword(const std::string& password) {
  std::string password_copy(password);
  mjohnson::common::TrimString(&password_copy);
  if (password_copy.empty()) {
    std::cout << std::endl
              << "ERROR: Your password cannot be blank." << std::endl
              << std::endl;

    return false;
  }

  return true;
}
}  // namespace shoutout
}  // namespace mjohnson
