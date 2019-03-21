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

View* Screen::CurrentView() { return this->view_; }

void Screen::Run(View* initial_view) {
  this->view_ = initial_view;
  while (this->view_ != nullptr) {
    View* new_view = this->view_->Display();
    if (new_view != this->view_) {
      // Only free the view if it didn't return itself
      delete this->view_;
    }
    this->view_ = new_view;
  }
}
}  // namespace shoutout
}  // namespace mjohnson
