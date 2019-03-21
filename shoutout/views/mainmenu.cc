// Copyright 2019 Michael Johnson

#include "shoutout/views/mainmenu.h"

#include <iostream>

namespace mjohnson {
namespace shoutout {
MainMenuView::MainMenuView() : error_message_("") {}

MainMenuView::MainMenuView(const std::string& error_message)
    : error_message_(error_message) {}

void MainMenuView::display() {
  std::cout << "Would display the main menu here." << std::endl;
}
}  // namespace shoutout
}  // namespace mjohnson
