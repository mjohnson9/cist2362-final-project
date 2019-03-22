// Copyright 2019 Michael Johnson

#include "shoutout/views/deleteshout.h"

#include <iostream>
#include <vector>

#include "shoutout/common.h"
#include "shoutout/screen.h"
#include "shoutout/views/profile.h"

namespace mjohnson {
namespace shoutout {
View* DeleteShoutView::Display() {
  Screen* screen = Screen::Get();

  std::string message;

  while (true) {
    screen->Clear();

    std::cout << "========== DELETE SHOUT ==========" << std::endl << std::endl;

    std::vector<Shout*>* shouts = this->user_->Shouts();

    if (shouts->empty()) {
      std::cout << "You have no shouts. Press enter to go back." << std::endl;
      std::string discard;
      std::getline(std::cin, discard);
      return new ProfileView(this->user_, this->user_);
    }

    size_t counter = 1;
    for (Shout* shout : *shouts) {
      std::cout << "[" << counter << "] " << shout->Content() << " - "
                << mjohnson::common::FriendlyTime(shout->Time()) << std::endl;
      counter += 1;
    }

    // Remove the extra addition so that counter is the maximum value
    counter -= 1;

    Shout* selected_shout = nullptr;

    while (true) {
      auto selection = mjohnson::common::RequestInput<std::string>(
          "Enter the number of a shout to delete it | [B]ack\n",
          std::bind(mjohnson::common::ValidateStringNotEmpty,
                    "\nERROR: You must enter a number or B for back.\n\n",
                    std::placeholders::_1));

      mjohnson::common::TrimString(&selection);
      mjohnson::common::LowerString(&selection);

      if (selection == "b") {
        return new ProfileView(this->user_, this->user_);
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

      selected_shout = (*shouts)[num_selection - 1];
      break;
    }

    this->user_->RemoveShout(selected_shout);
  }

  throw std::runtime_error(
      "Reached end of DeleteShoutView::Display -- this shouldn't happen");
}
}  // namespace shoutout
}  // namespace mjohnson
