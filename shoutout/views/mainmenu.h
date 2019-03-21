// Copyright 2019 Michael Johnson

#pragma once

#include <string>

#include "shoutout/views/view.h"

namespace mjohnson {
namespace shoutout {
/**
 * Provides the main menu. The main menu allows a user to register or login. It
 * can also display an error message from another view.
 */
class MainMenuView : public View {
 private:
  /**
   * The error message to be displayed with this main menu view.
   */
  std::string error_message_;

 public:
  MainMenuView();
  /**
   * Constructs a MainMenuView with a specified error message.
   * @param  error_message The error message to be displayed.
   */
  explicit MainMenuView(std::string error_message);
  ~MainMenuView() override = default;

  void display() override;
};
}  // namespace shoutout
}  // namespace mjohnson
