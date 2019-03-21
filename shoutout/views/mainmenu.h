// Copyright 2019 Michael Johnson

#pragma once

#include <string>

#include "shoutout/views/view.h"

namespace mjohnson {
namespace shoutout {
class MainMenuView : public View {
 private:
  std::string error_message_;

 public:
  MainMenuView();
  explicit MainMenuView(const std::string& error_message);
  ~MainMenuView() override = default;

  void display() override;
};
}  // namespace shoutout
}  // namespace mjohnson
