// Copyright 2019 Michael Johnson

#include "shoutout/views/shout.h"

#include <iostream>

#include "shoutout/common.h"
#include "shoutout/models/shout.h"
#include "shoutout/models/shoutout.h"
#include "shoutout/screen.h"
#include "shoutout/views/home.h"

namespace mjohnson {
namespace shoutout {
View* ShoutView::Display() {
  Screen* screen = Screen::Get();

  screen->Clear();

  std::cout << "========== SHOUT ==========" << std::endl << std::endl;

  auto content = mjohnson::common::RequestInput<std::string>(
      "What would you like to shout out?\n", ShoutView::ValidateShout);

  Shout* shout = new Shout(this->user_, content);
  this->user_->AddShout(shout);

  return new HomeView(this->user_);
}

bool ShoutView::ValidateShout(const std::string& username) {
  std::string username_copy(username);
  mjohnson::common::TrimString(&username_copy);
  if (username_copy.empty()) {
    std::cout << std::endl
              << "ERROR: Your Shout cannot be empty." << std::endl
              << std::endl;

    return false;
  }

  return true;
}
}  // namespace shoutout
}  // namespace mjohnson
