// Copyright 2019 Michael Johnson

#include "shoutout/views/finduser.h"

#include <algorithm>
#include <iostream>
#include <vector>

#include "shoutout/common.h"
#include "shoutout/models/shoutout.h"
#include "shoutout/screen.h"
#include "shoutout/views/home.h"

namespace mjohnson {
namespace shoutout {
View* FindUserView::Display() {
  Screen* screen = Screen::Get();
  ShoutOut* db = ShoutOut::Get();

  while (true) {
    screen->Clear();

    std::cout << "========== FIND USER ==========" << std::endl << std::endl;

    auto search_for = mjohnson::common::RequestInput<std::string>(
        "Who would you like to search for? ", FindUserView::ValidateSearch);

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

    std::sort(results.begin(), results.end(), User::UsernameCompare);

    return new HomeView(this->user_);
  }
}

bool FindUserView::ValidateSearch(const std::string& search) {
  std::string search_copy(search);
  mjohnson::common::TrimString(&search_copy);
  if (search_copy.empty()) {
    std::cout << std::endl
              << "ERROR: You must enter something to search for." << std::endl
              << std::endl;

    return false;
  }

  return true;
}
}  // namespace shoutout
}  // namespace mjohnson
