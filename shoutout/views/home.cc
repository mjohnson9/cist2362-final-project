// Copyright 2019 Michael Johnson

#include "shoutout/views/home.h"

#include <algorithm>
#include <vector>

#include "shoutout/common.h"
#include "shoutout/models/shout.h"
#include "shoutout/screen.h"
#include "shoutout/views/register.h"

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

    for (User* user : *this->user_->Following()) {
      for (Shout* shout : *user->Shouts()) {
        shouts.push_back(shout);
      }
    }

    std::sort(shouts.begin(), shouts.end(), ShoutTimeCompare);

    for (Shout* shout : shouts) {
      const time_t shout_time =
          std::chrono::system_clock::to_time_t(shout->Time());

      std::cout << "> " << shout->Owner()->Username() << ": "
                << shout->Content() << " - " << std::ctime(&shout_time)
                << std::endl;
    }
  }

  throw std::runtime_error(
      "Reached end of HomeView::Display -- this shouldn't happen");
}
}  // namespace shoutout
}  // namespace mjohnson
