// Copyright 2019 Michael Johnson

#include "shoutout/screen.h"
#include "shoutout/views/mainmenu.h"

int main() {
  mjohnson::shoutout::Screen* screen = mjohnson::shoutout::Screen::Get();
  screen->set_view(new mjohnson::shoutout::MainMenuView());
  return 0;
}
