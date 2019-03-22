// Copyright 2019 Michael Johnson

#include "shoutout/views/privateshouts.h"

#include <iostream>
#include <vector>

#include "shoutout/common.h"
#include "shoutout/screen.h"
#include "shoutout/views/home.h"
#include "shoutout/views/profile.h"

namespace mjohnson {
namespace shoutout {
View* PrivateShoutsView::Display() {
  if (this->constructing_shout_to_ != nullptr) {
    this->DisplayConstructShout();
    return new ProfileView(this->user_, this->constructing_shout_to_);
  }

  Screen* screen = Screen::Get();

  std::string message;

  while (true) {
    screen->Clear();
    std::cout << "========== PRIVATE SHOUTS ==========" << std::endl
              << std::endl;

    std::vector<PrivateShout*>* shouts = this->user_->PrivateShouts();

    if (shouts->empty()) {
      std::cout << "You have no private shouts. Press enter to go back."
                << std::endl
                << std::endl;

      std::string discard;
      std::getline(std::cin, discard);

      return new HomeView(this->user_);
    }

    for (PrivateShout* shout : *shouts) {
      std::cout << shout->From()->Username() << " -> "
                << shout->To()->Username() << ": " << shout->Content() << " - "
                << mjohnson::common::FriendlyTime(shout->Time()) << std::endl;
    }
    std::cout << std::endl;

    if (!message.empty()) {
      std::cout << message << std::endl;
      message = "";
    }

    std::cout << "[R]e-shout | [D]elete | [H]ome" << std::endl;

    auto original_choice =
        mjohnson::common::RequestInput<std::string>("", nullptr);

    std::string choice(original_choice);
    mjohnson::common::TrimString(&choice);
    mjohnson::common::LowerString(&choice);

    if (choice.empty()) {
      continue;
    }
    if (choice == "r") {
      this->DisplayReShout();
      return new HomeView(this->user_);
    }
    if (choice == "d") {
      this->DisplayDelete();
      continue;
    }
    if (choice == "h") {
      return new HomeView(this->user_);
    }

    message = "\"" + original_choice + "\" is not a valid choice.";
  }

  throw std::runtime_error(
      "Reached end of PrivateShoutsView::Display -- this shouldn't happen");
}

void PrivateShoutsView::DisplayConstructShout() {
  Screen* screen = Screen::Get();

  screen->Clear();

  User* to = this->constructing_shout_to_;

  std::cout << "========== SENDING PRIVATE SHOUT ==========" << std::endl
            << std::endl
            << "To: " << to->Username() << std::endl
            << std::endl;

  auto message = mjohnson::common::RequestInput<std::string>(
      "Message: ",
      std::bind(mjohnson::common::ValidateStringNotEmpty,
                "\nERROR: You must enter a message to be sent.\n\n",
                std::placeholders::_1));

  mjohnson::common::TrimString(&message);

  PrivateShout* from_shout = new PrivateShout(this->user_, to, message);
  this->user_->AddPrivateShout(from_shout);

  PrivateShout* to_shout = new PrivateShout(this->user_, to, message);
  to->AddPrivateShout(to_shout);
}

void PrivateShoutsView::DisplayDelete() {
  Screen* screen = Screen::Get();
  while (true) {
    screen->Clear();
    std::cout << "========== DELETE PRIVATE SHOUTS ==========" << std::endl
              << std::endl;

    std::vector<PrivateShout*>* shouts = this->user_->PrivateShouts();

    size_t counter = 1;
    for (PrivateShout* shout : *shouts) {
      std::cout << "[" << counter << "] " << shout->From()->Username() << " -> "
                << shout->To()->Username() << ": " << shout->Content() << " - "
                << mjohnson::common::FriendlyTime(shout->Time()) << std::endl;
      counter += 1;
    }
    std::cout << std::endl;

    // Subtract the 1 extra so that counter is the maximum possible number
    counter -= 1;

    PrivateShout* selected_shout = nullptr;

    while (true) {
      auto selection = mjohnson::common::RequestInput<std::string>(
          "Enter the number of a shout to delete | [B]ack\n",
          std::bind(mjohnson::common::ValidateStringNotEmpty,
                    "\nERROR: You must enter a number or B for back.\n\n",
                    std::placeholders::_1));

      mjohnson::common::TrimString(&selection);
      mjohnson::common::LowerString(&selection);

      if (selection == "b") {
        return;
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

    this->user_->RemovePrivateShout(selected_shout);
    if (this->user_->PrivateShouts()->empty()) {
      return;
    }
  }
}

void PrivateShoutsView::DisplayReShout() {
  Screen* screen = Screen::Get();
  while (true) {
    screen->Clear();
    std::cout << "========== RE-SHOUT PRIVATE SHOUTS ==========" << std::endl
              << std::endl;

    std::vector<PrivateShout*>* shouts = this->user_->PrivateShouts();

    size_t counter = 1;
    for (PrivateShout* shout : *shouts) {
      std::cout << "[" << counter << "] " << shout->From()->Username() << " -> "
                << shout->To()->Username() << ": " << shout->Content() << " - "
                << mjohnson::common::FriendlyTime(shout->Time()) << std::endl;
      counter += 1;
    }
    std::cout << std::endl;

    // Subtract the 1 extra so that counter is the maximum possible number
    counter -= 1;

    PrivateShout* selected_shout = nullptr;

    while (true) {
      auto selection = mjohnson::common::RequestInput<std::string>(
          "Enter the number of a shout to delete | [B]ack\n",
          std::bind(mjohnson::common::ValidateStringNotEmpty,
                    "\nERROR: You must enter a number or B for back.\n\n",
                    std::placeholders::_1));

      mjohnson::common::TrimString(&selection);
      mjohnson::common::LowerString(&selection);

      if (selection == "b") {
        return;
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

    Shout* new_shout = new Shout(
        this->user_, "Private shout: " + selected_shout->From()->Username() +
                         " to " + selected_shout->To()->Username() + ": " +
                         selected_shout->Content());
    this->user_->AddShout(new_shout);
    return;
  }
}
}  // namespace shoutout
}  // namespace mjohnson
