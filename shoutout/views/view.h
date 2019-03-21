// Copyright 2019 Michael Johnson

#pragma once

namespace mjohnson {
namespace shoutout {
/**
 * Abstract class defining functions common to all views.
 */
class View {
 public:
  /**
   * Displays the view and handles input relevant to the view. To switch to a
   * new view, this View would call Screen::Get()->set_view(). If display
   * returns, the program ends.
   */
  virtual void display() = 0;
  virtual ~View() = default;
};
}  // namespace shoutout
}  // namespace mjohnson
