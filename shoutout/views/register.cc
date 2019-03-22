// Copyright 2019 Michael Johnson

#include "shoutout/views/register.h"

#include "shoutout/common.h"
#include "shoutout/models/shoutout.h"
#include "shoutout/models/user.h"
#include "shoutout/screen.h"
#include "shoutout/views/home.h"

namespace mjohnson {
namespace shoutout {
bool ValidateUsername(const std::string& username);
bool ValidatePassword(const std::string& password);

View* RegisterView::Display() {
  Screen* screen = Screen::Get();

  while (true) {
    screen->Clear();

    std::cout << "========== REGISTER ==========" << std::endl << std::endl;

    if (this->username_.empty()) {
      auto username = mjohnson::common::RequestInput<std::string>(
          "What would you like your username to be? ",
          RegisterView::ValidateUsername);
      this->username_ = username;
      std::cout << std::endl;
    } else {
      std::cout << "Username: " << this->username_ << std::endl << std::endl;
    }

    if (this->password_mismatch_) {
      std::cout << "ERROR: The two passwords you entered did not match."
                << std::endl
                << std::endl;
      this->password_mismatch_ = false;
    }

    const auto password_validator = std::bind(
        mjohnson::common::ValidateStringNotEmpty,
        "\nERROR: Your password cannot be blank.\n\n", std::placeholders::_1);

    auto first_password = mjohnson::common::RequestInput<std::string>(
        "What would you like your password to be? ", password_validator);

    auto second_password = mjohnson::common::RequestInput<std::string>(
        "Please reenter your password: ", password_validator);

    if (first_password == second_password) {
      User* new_user = new User(this->username_, first_password);
      ShoutOut::Get()->AddUser(new_user);
      return new HomeView(new_user);
    }

    this->password_mismatch_ = true;
  }

  throw std::runtime_error(
      "Reached end of RegisterView::Display -- this shouldn't happen");
}

bool RegisterView::ValidateUsername(const std::string& username) {
  std::string username_copy(username);
  mjohnson::common::TrimString(&username_copy);
  if (username_copy.empty()) {
    std::cout << std::endl
              << "ERROR: Your username cannot be blank." << std::endl
              << std::endl;

    return false;
  }

  const std::string user_id = User::CreateUserId(username_copy);
  if (ShoutOut::Get()->GetUserById(user_id) != nullptr) {
    std::cout << std::endl
              << "ERROR: A user with that name already exists." << std::endl
              << std::endl;

    return false;
  }

  return true;
}
}  // namespace shoutout
}  // namespace mjohnson
