// Copyright 2019 Michael Johnson

#pragma once

#include "shoutout/views/view.h"

namespace mjohnson {
namespace shoutout {
/**
 * Manages the console screen.
 */
class Screen {
 private:
  /**
   * The current global Screen object.
   */
  static Screen* current_screen_;

  /**
   * The current view of this Screen.
   */
  View* view_;

 public:
  Screen();

  /**
   * Retrieves the current global Screen.
   * @return The current global Screen.
   */
  static Screen* Get() { return Screen::current_screen_; }

  /**
   * Clears the screen.
   */
  void Clear();

  /**
   * Retrieves the currently displayed View.
   * @return The currently displayed View.
   */
  View* CurrentView();

  /**
   * Starts the program's console system with the specified view.
   * @param initial_view The view to start with.
   */
  void Run(View* initial_view);
};

}  // namespace shoutout
}  // namespace mjohnson
