// Copyright 2019 Michael Johnson

#include "shoutout/common.h"

#include <algorithm>
#include <chrono>  // NOLINT(build/c++11)
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

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

// ValidateContinueResponse is a validation function for RequestInput. It
// validates that a response to the continue question can be used.
bool ValidateContinueResponse(const std::string& response);

bool RequestContinue() {
  return RequestContinue("Would you like to run the program again? [y/N] ");
}

bool RequestContinue(const std::string& prompt) {
  while (true) {
    auto response = RequestInput<std::string>(prompt, ValidateContinueResponse);
    if (response.empty()) {
      return false;  // The default option is to not continue
    }

    // Lowercase the response to make it easier to compare
    std::transform(response.begin(), response.end(), response.begin(),
                   ::tolower);
    if (response == "y" || response == "yes") {
      return true;
    }
    if (response == "n" || response == "no") {
      return false;
    }

    // We should never reach this point
    throw std::invalid_argument("response");
  }
}

bool ValidateContinueResponse(const std::string& response) {
  if (response.empty()) {
    // Shortcut; we allow empty responses
    return true;
  }

  std::string modified_response = response;  // Copy the response string

  // Lowercase the response to make it easier to compare
  std::transform(modified_response.begin(), modified_response.end(),
                 modified_response.begin(), ::tolower);

  const bool is_valid =
      (modified_response == "y" || modified_response == "yes" ||
       modified_response == "n" || modified_response == "no");
  if (!is_valid) {
    std::cout
        << response
        << " is an invalid response. Available responses are yes, y, no, or n."
        << std::endl
        << std::endl;
  }

  return is_valid;
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

}  // namespace common
}  // namespace mjohnson
