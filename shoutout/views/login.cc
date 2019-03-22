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
      "Username: ", std::bind(mjohnson::common::ValidateStringNotEmpty,
                              "\nERROR: Your username cannot be blank.\n\n",
                              std::placeholders::_1));

  auto password = mjohnson::common::RequestInput<std::string>(
      "Password: ", std::bind(mjohnson::common::ValidateStringNotEmpty,
                              "\nERROR: Your password cannot be blank.\n\n",
                              std::placeholders::_1));

  User* user = db->GetUserByName(username);
  if (user == nullptr || password != user->Password()) {
    return new MainMenuView(
        "ERROR: The provided username or password is incorrect.");
  }

  return new HomeView(user);
}
}  // namespace shoutout
}  // namespace mjohnson
