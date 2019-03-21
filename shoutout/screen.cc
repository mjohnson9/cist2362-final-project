// Copyright 2019 Michael Johnson

#include "shoutout/screen.h"

#include <cstdlib>

namespace mjohnson {
namespace shoutout {
Screen* Screen::current_screen_ = new Screen();

void Screen::Clear() {
  // It feels nasty forking a process to clear the screen, but it's a better
  // alternative to including something like ncurses
#if defined(_WIN32) || defined(_WIN64)
  system("CLS");
#elif defined(unix) || defined(__unix__) || defined(__unix) || \
    (defined(__APPLE__) && defined(__MACH__))
  system("clear");
#else
#error "Operating system not supported"
#endif  // End of OS detection
}

Screen::Screen() { this->view_ = nullptr; }

View* Screen::view() { return this->view_; }

void Screen::set_view(View* view) {
  // Free the memory held by the previous view
  // We don't check if it's null because deleting nullptr has no effect.
  delete this->view_;

  this->view_ = view;
  this->view_->display();
}
}  // namespace shoutout
}  // namespace mjohnson
