// Copyright 2019 Michael Johnson

#include "shoutout/views/profile.h"

#include <chrono>  // NOLINT(build/c++11)
#include <iostream>
#include <string>

#include "shoutout/common.h"
#include "shoutout/screen.h"
#include "shoutout/views/deleteshout.h"
#include "shoutout/views/home.h"
#include "shoutout/views/privateshouts.h"
#include "shoutout/views/userlist.h"

namespace mjohnson {
namespace shoutout {
View* ProfileView::Display() {
  User* user = this->viewing_user_;
  bool own_profile =
      (this->logged_in_user_->UserId() == this->viewing_user_->UserId());
  Screen* screen = Screen::Get();

  std::string message;

  while (true) {
    screen->Clear();

    if (own_profile) {
      std::cout << "========== VIEWING PROFILE ==========" << std::endl
                << std::endl;
    } else {
      std::cout << "========== VIEWING PROFILE: " << user->Username()
                << " ==========" << std::endl
                << std::endl;
    }

    if (!user->Shouts()->empty()) {
      this->PrintShoutList(user->Shouts());
      std::cout << std::endl;
    } else {
      if (own_profile) {
        std::cout << "You have no shouts." << std::endl << std::endl;
      } else {
        std::cout << this->viewing_user_->Username() << " has no shouts."
                  << std::endl
                  << std::endl;
      }
    }

    if (!message.empty()) {
      std::cout << message << std::endl;
    }

    bool is_following = false;
    if (!own_profile) {
      std::string user_id = this->viewing_user_->UserId();
      for (User* user : *this->logged_in_user_->Following()) {
        if (user->UserId() == user_id) {
          is_following = true;
          break;
        }
      }

      if (!is_following) {
        std::cout << "[F]ollow | ";
      } else {
        std::cout << "[U]nfollow | ";
      }
      std::cout << "[P]rivate Shout | ";
    }
    std::cout << "F[o]llowers | Follow[i]ng";
    if (own_profile && !this->viewing_user_->Shouts()->empty()) {
      std::cout << " | [D]elete shout";
    }
    std::cout << " | [H]ome" << std::endl;

    std::string chosen_option =
        mjohnson::common::RequestInput<std::string>("", nullptr);
    std::string original_choice(
        chosen_option);  // Copy the string before we modify it
    mjohnson::common::TrimString(&chosen_option);  // Trim the whitespace
    mjohnson::common::LowerString(
        &chosen_option);  // Convert the string to lowercase for easy comparison

    if (!own_profile) {
      if (!is_following && chosen_option == "f") {
        this->logged_in_user_->AddFollowing(this->viewing_user_);
        this->viewing_user_->AddFollower(this->logged_in_user_);
        continue;
      }
      if (is_following && chosen_option == "u") {
        this->logged_in_user_->RemoveFollowing(this->viewing_user_);
        this->viewing_user_->RemoveFollower(this->logged_in_user_);
        continue;
      }
      if (chosen_option == "p") {
        return new PrivateShoutsView(this->logged_in_user_,
                                     this->viewing_user_);
      }
    }
    if (chosen_option == "o") {
      return new UserListView(this->logged_in_user_, this->viewing_user_,
                              UserListView::kFollowersList);
    }
    if (chosen_option == "i") {
      return new UserListView(this->logged_in_user_, this->viewing_user_,
                              UserListView::kFollowingList);
    }
    if (own_profile && chosen_option == "d" &&
        !this->viewing_user_->Shouts()->empty()) {
      return new DeleteShoutView(this->logged_in_user_);
    }
    if (chosen_option == "h") {
      return new HomeView(this->logged_in_user_);
    }

    message = "ERROR: \"" + original_choice + "\" is not a valid menu choice.";
  }

  throw std::runtime_error(
      "Reached end of ProfileView::Display -- this shouldn't happen");
}

void ProfileView::PrintShoutList(std::vector<Shout*>* shouts) {
  for (Shout* shout : *shouts) {
    std::cout << "> " << shout->Content() << " - "
              << mjohnson::common::FriendlyTime(shout->Time()) << std::endl;
  }
}
}  // namespace shoutout
}  // namespace mjohnson
