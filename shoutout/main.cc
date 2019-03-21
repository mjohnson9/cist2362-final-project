// Copyright 2019 Michael Johnson

#include <iostream>
#include <stdexcept>

#include "shoutout/common.h"
#include "shoutout/models/shoutout.h"
#include "shoutout/screen.h"
#include "shoutout/views/mainmenu.h"

void usage(char* program) {
  std::cerr << "Usage: " << program << " [data directory]" << std::endl;
}

int main(int argc, char* argv[]) {
  if (argc > 2) {
    usage(argv[0]);
    return 1;
  }

  std::string data_directory = "shoutout-data";
  if (argc == 2) {
    data_directory = std::string(argv[1]);
    mjohnson::common::TrimString(&data_directory);
    if (data_directory.empty()) {
      usage(argv[0]);
      return 1;
    }
  }

  try {
    mjohnson::shoutout::ShoutOut* shoutout =
        mjohnson::shoutout::ShoutOut::CreateGlobalShoutOut(data_directory);
    shoutout->LoadFromDisk();

    mjohnson::shoutout::Screen* screen = mjohnson::shoutout::Screen::Get();
    screen->SetView(new mjohnson::shoutout::MainMenuView());
  } catch (const std::exception& ex) {
    std::cerr << "Program terminated with an exception:" << std::endl
              << ex.what() << std::endl;
    return 1;
  }
  return 0;
}
