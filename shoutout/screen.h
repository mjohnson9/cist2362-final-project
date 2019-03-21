// Copyright 2019 Michael Johnson

#pragma once

#include "shoutout/views/view.h"

namespace mjohnson {
namespace shoutout {
class Screen {
 private:
  static Screen* current_screen_;

  View* view_;

 public:
  Screen();

  static Screen* Get() { return Screen::current_screen_; }

  View* view();
  void set_view(View* view);
};

}  // namespace shoutout
}  // namespace mjohnson
