// Copyright 2019 Michael Johnson

#include "shoutout/screen.h"

namespace mjohnson {
namespace shoutout {
Screen* Screen::current_screen_ = new Screen();

Screen::Screen() { this->view_ = nullptr; }

View* Screen::view() { return this->view_; }

void Screen::set_view(View* view) {
  // Free the memory held by the previous view
  // We don't check if it's null, because deleting nullptr has no effect.
  delete this->view_;

  this->view_ = view;
  this->view_->display();
}
}  // namespace shoutout
}  // namespace mjohnson
