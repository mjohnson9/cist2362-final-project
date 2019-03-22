// Copyright 2019 Michael Johnson

#include "shoutout/views/userlist.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

#include "shoutout/common.h"
#include "shoutout/models/shoutout.h"
#include "shoutout/screen.h"
#include "shoutout/views/home.h"
#include "shoutout/views/profile.h"

namespace mjohnson {
namespace shoutout {
bool IsDigits(const std::string& str);

View* UserListView::Display() {
  Screen* screen = Screen::Get();

  std::string message;

  while (true) {
    screen->Clear();
    this->DisplayHeader();

    std::vector<User*>* users = nullptr;
    if (this->list_type_ == UserListView::kFollowersList) {
      users = this->viewing_user_->Followers();
    } else if (this->list_type_ == UserListView::kFollowingList) {
      users = this->viewing_user_->Following();
    }

    bool own_list =
        (this->logged_in_user_->UserId() == this->viewing_user_->UserId());

    if (users->empty()) {
      if (own_list) {
        std::cout << "You ";
        if (this->list_type_ == UserListView::kFollowersList) {
          std::cout << "have no followers.";
        } else if (this->list_type_ == UserListView::kFollowingList) {
          std::cout << "are not following anyone.";
        }
      } else {
        std::cout << this->viewing_user_->Username() << " ";
        if (this->list_type_ == UserListView::kFollowersList) {
          std::cout << "has no followers.";
        } else if (this->list_type_ == UserListView::kFollowingList) {
          std::cout << "is not following anyone.";
        }
      }

      std::cout << " Press enter to return to ";
      if (own_list) {
        std::cout << "your profile.";
      } else {
        std::cout << this->viewing_user_->Username() << "'s profile.";
      }
      std::cout << std::endl;

      std::string discard;
      std::getline(std::cin, discard);

      return new ProfileView(this->logged_in_user_, this->viewing_user_);
    }

    std::sort(users->begin(), users->end(), User::UsernameCompare);

    size_t counter = 1;
    for (User* user : *users) {
      std::cout << "[" << counter << "] " << user->Username() << std::endl;
      counter += 1;
    }
    std::cout << std::endl;

    if (!message.empty()) {
      std::cout << message << std::endl;
      message = "";
    }

    std::cout << "Enter a number corresponding to a user to go to that user's "
                 "profile";
    if (own_list) {
      std::cout << " | [D]elete";
    }
    std::cout << " | [S]earch | [B]ack" << std::endl;

    auto original_selection =
        mjohnson::common::RequestInput<std::string>("", nullptr);

    std::string selection(original_selection);
    mjohnson::common::TrimString(&selection);
    mjohnson::common::LowerString(&selection);

    if (selection.empty()) {
      continue;
    }
    if (own_list && selection == "d") {
      this->DisplayDelete();
      continue;
    }
    if (selection == "s") {
      View* search_view = this->DisplaySearch();
      if (search_view != nullptr) {
        return search_view;
      }
      continue;
    }
    if (selection == "b") {
      return new ProfileView(this->logged_in_user_, this->viewing_user_);
    }
    if (mjohnson::common::IsDigits(selection)) {
      size_t users_size = users->size();
      int64_t selection_number = atoll(selection.c_str());
      if (selection_number < 1 ||
          static_cast<size_t>(selection_number) > users_size) {
        if (users_size > 1) {
          message = "Your selection must be between 1 and " +
                    std::to_string(users->size()) + ".";
        } else {
          message = "Your selection must be 1 or a letter.";
        }
        continue;
      }

      User* selected_user = (*users)[selection_number - 1];
      return new ProfileView(this->logged_in_user_, selected_user);
    }

    message = "\"" + original_selection + "\" is not a valid selection.";
  }

  throw std::runtime_error(
      "Reached end of UserListView::Display -- this shouldn't happen");
}

void UserListView::DisplayDelete() {
  Screen* screen = Screen::Get();
  while (true) {
    screen->Clear();
    this->DisplayHeader();

    std::string list_name;
    std::vector<User*>* users = nullptr;
    if (this->list_type_ == UserListView::kFollowingList) {
      list_name = "following";
      users = this->viewing_user_->Following();
    } else if (this->list_type_ == UserListView::kFollowersList) {
      list_name = "followers";
      users = this->viewing_user_->Followers();
    }

    size_t counter = 1;
    for (User* user : *users) {
      std::cout << "[" << counter << "] " << user->Username() << std::endl;
      counter += 1;
    }

    User* selected_user = this->PromptUserSelection(
        users, "Enter the number of a user to remove them from your " +
                   list_name + " list | [B]ack\n");

    if (selected_user == nullptr) {
      return;
    }

    if (this->list_type_ == UserListView::kFollowingList) {
      this->viewing_user_->RemoveFollowing(selected_user);
      selected_user->RemoveFollower(this->viewing_user_);

      if (this->viewing_user_->Following()->empty()) {
        // Nothing left to delete
        return;
      }
    } else if (this->list_type_ == UserListView::kFollowersList) {
      this->viewing_user_->RemoveFollower(selected_user);
      selected_user->RemoveFollowing(this->viewing_user_);

      if (this->viewing_user_->Followers()->empty()) {
        // Nothing left to delete
        return;
      }
    }
  }
}

View* UserListView::DisplaySearch() {
  Screen* screen = Screen::Get();
  while (true) {
    screen->Clear();
    this->DisplayHeader();

    auto search_for = mjohnson::common::RequestInput<std::string>(
        "Who would you like to search for? ",
        std::bind(mjohnson::common::ValidateStringNotEmpty,
                  "\nERROR: You must enter something to search for.\n\n",
                  std::placeholders::_1));
    std::string original_search_for(search_for);

    mjohnson::common::TrimString(&search_for);
    mjohnson::common::LowerString(&search_for);

    screen->Clear();
    this->DisplayHeader();
    std::cout << "Searching for: " << original_search_for << std::endl
              << std::endl;

    std::vector<User*>* pool = nullptr;
    if (this->list_type_ == UserListView::kFollowingList) {
      pool = this->viewing_user_->Following();
    } else if (this->list_type_ == UserListView::kFollowersList) {
      pool = this->viewing_user_->Followers();
    }

    std::vector<User*> users;

    size_t counter = 1;
    for (User* user : *pool) {
      std::string username = user->Username();
      mjohnson::common::LowerString(&username);

      size_t find_result = username.find(search_for);
      if (find_result != std::string::npos) {
        users.push_back(user);
        std::cout << "[" << counter << "] " << user->Username() << std::endl;
        counter += 1;
      }
    }
    if (users.empty()) {
      std::cout << "No results found. Press enter to go back." << std::endl;
      std::string discard;
      std::getline(std::cin, discard);
      return nullptr;
    }

    std::cout << std::endl;

    User* selected_user = this->PromptUserSelection(
        &users, "Enter the number of a user to go to their profile | [B]ack\n");

    if (selected_user == nullptr) {
      return nullptr;
    }

    return new ProfileView(this->logged_in_user_, selected_user);
  }
}

void UserListView::DisplayHeader() {
  bool own_list =
      (this->logged_in_user_->UserId() == this->viewing_user_->UserId());

  if (own_list) {
    if (this->list_type_ == UserListView::kFollowingList) {
      std::cout << "========== FOLLOWING ==========";
    } else if (this->list_type_ == UserListView::kFollowersList) {
      std::cout << "========== FOLLOWERS ==========";
    }
  } else {
    if (this->list_type_ == UserListView::kFollowingList) {
      std::cout << "========== FOLLOWING: " << this->viewing_user_->Username()
                << " ==========";
    } else if (this->list_type_ == UserListView::kFollowersList) {
      std::cout << "========== FOLLOWERS: " << this->viewing_user_->Username()
                << " ==========";
    }
  }
  std::cout << std::endl << std::endl;
}

User* UserListView::PromptUserSelection(std::vector<User*>* users,
                                        const std::string& prompt) {
  const size_t size = users->size();
  while (true) {
    auto selection = mjohnson::common::RequestInput<std::string>(
        prompt, std::bind(mjohnson::common::ValidateStringNotEmpty,
                          "\nERROR: You must enter a number or B for back.\n\n",
                          std::placeholders::_1));

    mjohnson::common::TrimString(&selection);
    mjohnson::common::LowerString(&selection);

    if (selection == "b") {
      return nullptr;
    }
    if (!mjohnson::common::IsDigits(selection)) {
      if (size == 1) {
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
    if (num_selection < 1 || static_cast<size_t>(num_selection) > size) {
      if (size > 1) {
        std::cout << std::endl
                  << "ERROR: Your selection must be between 1 and " << size
                  << "." << std::endl
                  << std::endl;
      } else {
        std::cout << std::endl
                  << "ERROR: You must enter 1 or B for back." << std::endl
                  << std::endl;
      }
      continue;
    }

    return (*users)[num_selection - 1];
  }
}
}  // namespace shoutout
}  // namespace mjohnson
