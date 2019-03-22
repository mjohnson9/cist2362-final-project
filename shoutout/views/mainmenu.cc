// Copyright 2019 Michael Johnson

#include "shoutout/views/mainmenu.h"

#include <iostream>
#include <string>
#include <utility>

#include "shoutout/common.h"
#include "shoutout/screen.h"
#include "shoutout/views/login.h"
#include "shoutout/views/register.h"

namespace mjohnson {
namespace shoutout {
MainMenuView::MainMenuView() : message_("") {}

MainMenuView::MainMenuView(std::string message)
    : message_(std::move(message)) {}

View* MainMenuView::Display() {
  Screen* screen = Screen::Get();

  while (true) {  // Make an infinite loop; we'll break out when
                  // we're ready to continue
    screen->Clear();
    std::cout << "========== SHOUTOUT.COM ==========" << std::endl;
    if (!this->message_.empty()) {
      std::cout << this->message_ << std::endl << std::endl;
      this->message_ = "";
    }
    std::cout << "Please choose an option:" << std::endl
              << "[l] Login" << std::endl
              << "[r] Register" << std::endl
              << "[q] Quit" << std::endl
              << std::endl;

    std::string chosen_option =
        mjohnson::common::RequestInput<std::string>("", nullptr);
    std::string original_choice(
        chosen_option);  // Copy the string before we modify it
    mjohnson::common::TrimString(&chosen_option);  // Trim the whitespace
    mjohnson::common::LowerString(
        &chosen_option);  // Convert the string to lowercase for easy comparison

    if (chosen_option.empty()) {
      continue;
    }
    if (chosen_option == "l") {
      return new LoginView();
    }
    if (chosen_option == "r") {
      return new RegisterView();
    }
    if (chosen_option == "q") {
      return nullptr;
    }

    this->message_ =
        "ERROR: \"" + original_choice + "\" is not a valid menu choice.";
  }

  throw std::runtime_error(
      "Reached end of MainMenuView::Display -- this shouldn't happen");
}
}  // namespace shoutout
}  // namespace mjohnson
