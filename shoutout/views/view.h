// Copyright 2019 Michael Johnson

#pragma once

namespace mjohnson {
namespace shoutout {
// View is an abstract class defining functions common to any views.
class View {
 public:
  virtual void display() = 0;
  virtual ~View() {}
};
}  // namespace shoutout
}  // namespace mjohnson
