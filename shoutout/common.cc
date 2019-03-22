// Copyright 2019 Michael Johnson

#include "shoutout/common.h"

#include <algorithm>
#include <chrono>  // NOLINT(build/c++11)
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

namespace mjohnson {
namespace common {
// This is a specialization for string. It reads entire lines instead of a
// single word.
template <>
std::string RequestInput<std::string>(
    const std::string& prompt,
    const std::function<bool(std::string)>& validator) {
  bool valid = true;
  std::string response;
  do {
    std::cout << prompt;
    std::getline(std::cin, response);

    if (validator) {  // Validator has a bool operator that tells us whether or
                      // not the function is empty
      valid = validator(response);
    } else {
      valid = true;
    }
  } while (!valid);

  return response;
}

void ClearInputWhitespace() {
  char c = static_cast<char>(std::cin.peek());
  if (std::isspace(c) == 0) {
    // The next character isn't whitespace; leave it alone
    return;
  }
  std::cin.ignore();  // Ignore the whitespace
}

void ClearInvalidInput() {
  std::cin.clear();  // clear the error from cin
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                  '\n');  // Ignore all of the user input currently in the
                          // buffer up to the next new line.
}

bool ValidateStringNotEmpty(const std::string& message,
                            const std::string& str) {
  std::string str_copy(str);
  TrimString(&str_copy);
  if (str.empty()) {
    std::cout << message;
    return false;
  }

  return true;
}

void TrimString(std::string* str) {
  str->erase(str->begin(), std::find_if(str->begin(), str->end(), [](int c) {
               return std::isspace(c) == 0;
             }));  // Erase the string from the beginning until the last
                   // whitespace character
  str->erase(std::find_if(str->rbegin(), str->rend(),
                          [](int c) { return std::isspace(c) == 0; })
                 .base(),
             str->end());  // Erase the string from the last whitespace
                           // character (searching right to left) to the end
}

void LowerString(std::string* str) {
  std::transform(str->begin(), str->end(), str->begin(), ::tolower);
}

bool IsDigits(const std::string& str) {
  for (const char c : str) {
    if (std::isdigit(c) == 0) {
      return false;
    }
  }

  return true;
}

std::string FriendlyTime(std::chrono::system_clock::time_point time) {
  auto current = std::chrono::system_clock::now();
  auto ago = current - time;
  if (ago < std::chrono::minutes(2)) {
    return "right now";
  }
  if (ago < std::chrono::hours(1)) {
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(ago);
    return std::to_string(minutes.count()) + " minutes ago";
  }
  if (ago < std::chrono::hours(24)) {
    auto hours = std::chrono::duration_cast<std::chrono::hours>(ago);
    return std::to_string(hours.count()) + " hours ago";
  }

  const time_t c_time_current = std::chrono::system_clock::to_time_t(current);
  std::tm current_localtime = *std::localtime(&c_time_current);
  // Copy so that we're not using the internal static pointer from localtime
  current_localtime = std::tm(current_localtime);

  const time_t c_time = std::chrono::system_clock::to_time_t(time);
  std::tm* localtime = std::localtime(&c_time);

  const char* time_format;
  if (ago < std::chrono::hours(24 * 7)) {
    time_format = static_cast<const char*>("%a %I:%M");
  } else if (localtime->tm_year == current_localtime.tm_year) {
    time_format = static_cast<const char*>("%b %d %I:%M");
  } else {
    time_format = static_cast<const char*>("%b %d %Y %I:%M");
  }

  const size_t kBufSize = 512;
  char* buf = new char[kBufSize];
  size_t len = strftime(buf, kBufSize, time_format, localtime);

  const std::string formatted_time(buf, len);

  return formatted_time;
}  // namespace common
}  // namespace common
}  // namespace mjohnson
