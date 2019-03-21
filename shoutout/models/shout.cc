// Copyright 2019 Michael Johnson

#include "shoutout/models/shout.h"

namespace mjohnson {
namespace shoutout {
bool ShoutTimeCompare(Shout* shout1, Shout* shout2) {
  return shout1->Time() < shout2->Time();
}
}  // namespace shoutout
}  // namespace mjohnson
