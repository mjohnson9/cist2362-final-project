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
      "What would you like to shout out?\n",
      std::bind(mjohnson::common::ValidateStringNotEmpty,
                "\nERROR: Your Shout cannot be empty.\n\n",
                std::placeholders::_1));

  Shout* shout = new Shout(this->user_, content);
  this->user_->AddShout(shout);

  return new HomeView(this->user_);
}
}  // namespace shoutout
}  // namespace mjohnson
