// Copyright 2019 Michael Johnson

#pragma once

#include <string>

namespace mjohnson {
namespace shoutout {
/**
 * An exception that is thrown when data is corrupted. Data is corrupted if only
 * part of the required data exists.
 */
class DataCorruptedException : public std::runtime_error {
 public:
  explicit DataCorruptedException(const std::string& message)
      : std::runtime_error("Data corruption: " + message) {}
};
}  // namespace shoutout
}  // namespace mjohnson
