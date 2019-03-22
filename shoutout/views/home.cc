// Copyright 2019 Michael Johnson

#include "shoutout/views/home.h"

#include <algorithm>
#include <vector>

#include "shoutout/common.h"
#include "shoutout/models/shout.h"
#include "shoutout/screen.h"
#include "shoutout/views/finduser.h"
#include "shoutout/views/mainmenu.h"
#include "shoutout/views/shout.h"

namespace mjohnson {
namespace shoutout {
View* HomeView::Display() {
  Screen* screen = Screen::Get();

  while (true) {  // Make an infinite loop; we'll break out when
                  // we're ready to continue
    screen->Clear();
    std::cout << "========== SHOUTOUT.COM ==========" << std::endl;
    std::cout << "Logged in as: " << this->user_->Username() << std::endl
              << std::endl;

    std::vector<Shout*> shouts;

    for (Shout* shout : *this->user_->Shouts()) {
      shouts.push_back(shout);
    }

    for (User* user : *this->user_->Following()) {
      for (Shout* shout : *user->Shouts()) {
        shouts.push_back(shout);
      }
    }

    std::sort(shouts.begin(), shouts.end(), Shout::TimeCompare);

    for (Shout* shout : shouts) {
      const time_t shout_time =
          std::chrono::system_clock::to_time_t(shout->Time());

      std::cout << "> " << shout->Owner()->Username() << ": "
                << shout->Content() << " - " << std::ctime(&shout_time)
                << std::endl;
    }
    std::cout << std::endl;

    if (!this->message_.empty()) {
      std::cout << this->message_ << std::endl;
    }

    std::cout << "[S]hout | [P]rivate Shouts | [F]ind other users | [E]xit"
              << std::endl;

    std::string chosen_option =
        mjohnson::common::RequestInput<std::string>("", nullptr);
    std::string original_choice(
        chosen_option);  // Copy the string before we modify it
    mjohnson::common::TrimString(&chosen_option);  // Trim the whitespace
    mjohnson::common::LowerString(
        &chosen_option);  // Convert the string to lowercase for easy comparison

    if (chosen_option == "s") {
      return new ShoutView(this->user_);
    }
    if (chosen_option == "p") {
      std::cout << "Would take to private shouts view" << std::endl;
      continue;
    }
    if (chosen_option == "f") {
      return new FindUserView(this->user_);
    }
    if (chosen_option == "e") {
      return new MainMenuView();
    }

    this->message_ =
        "ERROR: \"" + original_choice + "\" is not a valid menu choice.";
  }

  throw std::runtime_error(
      "Reached end of HomeView::Display -- this shouldn't happen");
}
}  // namespace shoutout
}  // namespace mjohnson
