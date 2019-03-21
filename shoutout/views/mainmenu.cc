// Copyright 2019 Michael Johnson

#include "shoutout/views/mainmenu.h"

#include <iostream>
#include <string>
#include <utility>

#include "shoutout/common.h"
#include "shoutout/screen.h"

namespace mjohnson {
namespace shoutout {
MainMenuView::MainMenuView() : error_message_("") {}

MainMenuView::MainMenuView(std::string error_message)
    : error_message_(std::move(error_message)) {}

void MainMenuView::display() {
  std::string chosen_option;
  while (true) {  // Make an infinite loop; we'll break out when
                  // we're ready to continue
    Screen* screen = Screen::Get();
    screen->Clear();
    std::cout << "========== SHOUTOUT.COM ==========" << std::endl;
    if (!this->error_message_.empty()) {
      std::cout << this->error_message_ << std::endl << std::endl;
    }
    std::cout << "Please choose an option:" << std::endl
              << "[l] Login" << std::endl
              << "[r] Register" << std::endl
              << std::endl;

    chosen_option = mjohnson::common::RequestInput<std::string>("", nullptr);
    std::string original_choice(
        chosen_option);  // Copy the string before we modify it
    mjohnson::common::TrimString(&chosen_option);  // Trim the whitespace
    mjohnson::common::LowerString(
        &chosen_option);  // Convert the string to lowercase for easy comparison

    if (chosen_option == "l" || chosen_option == "r") {
      break;
    }

    this->error_message_ =
        "ERROR: \"" + original_choice + "\" is not a valid menu choice.";
  }

  std::cout << "Would send to view for " << chosen_option << std::endl;
}
}  // namespace shoutout
}  // namespace mjohnson
