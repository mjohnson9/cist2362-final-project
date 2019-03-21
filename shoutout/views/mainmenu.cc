// Copyright 2019 Michael Johnson

#include "shoutout/views/mainmenu.h"

#include <iostream>
#include <utility>

namespace mjohnson {
namespace shoutout {
MainMenuView::MainMenuView() : error_message_("") {}

MainMenuView::MainMenuView(std::string error_message)
    : error_message_(std::move(error_message)) {}

void MainMenuView::display() {
  std::cout << "Would display the main menu here." << std::endl;
}
}  // namespace shoutout
}  // namespace mjohnson
