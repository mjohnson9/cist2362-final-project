// Copyright 2019 Michael Johnson

#include "shoutout/views/finduser.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

#include "shoutout/common.h"
#include "shoutout/models/shoutout.h"
#include "shoutout/screen.h"
#include "shoutout/views/home.h"
#include "shoutout/views/profile.h"

namespace mjohnson {
namespace shoutout {
bool IsDigits(const std::string& str);

View* FindUserView::Display() {
  Screen* screen = Screen::Get();
  ShoutOut* db = ShoutOut::Get();

  while (true) {
    screen->Clear();

    std::cout << "========== FIND USER ==========" << std::endl << std::endl;

    auto search_for = mjohnson::common::RequestInput<std::string>(
        "Who would you like to search for? ",
        std::bind(mjohnson::common::ValidateStringNotEmpty,
                  "\nERROR: You must enter something to search for.\n\n",
                  std::placeholders::_1));

    screen->Clear();

    std::cout << "========== SEARCHING FOR: " << search_for
              << " ==========" << std::endl
              << std::endl;

    mjohnson::common::LowerString(&search_for);

    std::vector<User*> results;

    for (User* user : *db->Users()) {
      std::string username = user->Username();
      mjohnson::common::LowerString(&username);

      size_t find_result = username.find(search_for);
      if (find_result != std::string::npos) {
        results.push_back(user);
      }
    }

    if (results.empty()) {
      std::cout << "No results found. Press enter to return to the main menu."
                << std::endl;
      std::string discard;
      std::getline(std::cin, discard);
      return new HomeView(this->user_);
    }

    std::sort(results.begin(), results.end(), User::UsernameCompare);

    size_t counter = 1;
    for (User* user : results) {
      std::cout << "[" << counter << "] " << user->Username() << std::endl;
      counter += 1;
    }

    counter -= 1;  // Go back one so that counter is the maximum possible value

    User* selected_user = nullptr;

    while (true) {
      auto selection = mjohnson::common::RequestInput<std::string>(
          "Enter the number of a user to view their profile | [B]ack\n",
          std::bind(mjohnson::common::ValidateStringNotEmpty,
                    "\nERROR: You must enter a number or B for back.\n\n",
                    std::placeholders::_1));

      mjohnson::common::TrimString(&selection);
      mjohnson::common::LowerString(&selection);

      if (selection == "b") {
        return new HomeView(this->user_);
      }
      if (!mjohnson::common::IsDigits(selection)) {
        if (counter == 1) {
          std::cout << std::endl
                    << "ERROR: You must enter 1 or B for back." << std::endl
                    << std::endl;
        } else {
          std::cout << std::endl
                    << "ERROR: You must enter a number or B for back."
                    << std::endl
                    << std::endl;
        }
        continue;
      }

      int64_t num_selection = atoll(selection.c_str());
      if (num_selection < 1 || static_cast<size_t>(num_selection) > counter) {
        if (counter > 1) {
          std::cout << std::endl
                    << "ERROR: Your selection must be between 1 and " << counter
                    << "." << std::endl
                    << std::endl;
        } else {
          std::cout << std::endl
                    << "ERROR: You must enter 1 or B for back." << std::endl
                    << std::endl;
        }
        continue;
      }

      selected_user = results[num_selection - 1];
      break;
    }

    return new ProfileView(this->user_, selected_user);
  }

  throw std::runtime_error(
      "Reached end of FindUserView::Display -- this shouldn't happen");
}
}  // namespace shoutout
}  // namespace mjohnson
