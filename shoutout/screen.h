// Copyright 2019 Michael Johnson

#pragma once

namespace mjohnson {
namespace shoutout {
class Screen {
 private:
  View* view_;

 public:
  Screen();

  View* View();
  void SetView(View* view);
};
}  // namespace shoutout
}  // namespace mjohnson
