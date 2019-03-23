// Copyright 2019 Michael Johnson

#pragma once

#include <algorithm>
#include <chrono>  // NOLINT(build/c++11)
#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace mjohnson {
namespace common {
/**
 * A list of characters forbidden from being used in filesystem object names.
 * This is used to create filesystem-friendly user IDs.
 */
const std::vector<char> kFilesystemForbiddenCharacters =
    []() -> std::vector<char> {
  std::string temporaryForbiddenCharacters = "/?<>\\:*|\"^";
  std::sort(temporaryForbiddenCharacters.begin(),
            temporaryForbiddenCharacters.end());
  std::vector<char> forbiddenCharactersVector(
      temporaryForbiddenCharacters.begin(), temporaryForbiddenCharacters.end());
  return forbiddenCharactersVector;
}();

/**
 * Clears the trailing whitespace after a read from cin.
 */
void ClearInputWhitespace();

/**
 * Clears invalid input form cin and resets the error flags.
 */
void ClearInvalidInput();

/**
 * Requests input from the user using a specified prompt and validator. The
 * input is first validated against T. Then, if validator is not null, the input
 * is validated against the provided validator function. The validator is
 * responsible for displaying an error message if the input is invalid. If the
 * input passes both of these validations, it is returned to the caller.
 *
 * @param  prompt The message to display to the user before waiting for input.
 * @param  validator The function to use to validate user input.
 * @returns Validated user input of the specified type.
 */
template <typename T>
T RequestInput(const std::string& prompt,
               const std::function<bool(T)>& validator) {
  bool valid = true;
  T response;
  do {
    std::cout << prompt;
    std::cin >> response;
    ClearInputWhitespace();

    if (std::cin.fail()) {  // cin.fail returns true when we attempt to extract
                            // a type from the stream, but the data that the
                            // user entered cannot be converted to that type.
      std::cout << "You have given an invalid answer. Please answer the "
                   "question with a valid input."
                << std::endl
                << std::endl;
      valid = false;
      ClearInvalidInput();
      continue;  // Fail fast and attempt another prompt
    }

    if (validator) {  // Validator has a bool operator that tells us whether or
                      // not the function is empty
      valid = validator(response);
    } else {
      valid = true;
    }
  } while (!valid);

  return response;
}

/**
 * The string specialization of RequestInput returns an entire line of input
 * from the user. The input is validated against the validator provided (if it's
 * not null). The validator is responsible for displaying an error message if
 * the input is invalid. If the input passes the validator, it is returned to
 * the caller.
 *
 * @param  prompt The message to display to the user before waiting for input.
 * @param  validator The function to use to validate user input.
 * @returns Validated user input of the specified type.
 */
template <>
std::string RequestInput<std::string>(
    const std::string& prompt,
    const std::function<bool(std::string)>& validator);

bool ValidateStringNotEmpty(const std::string& message, const std::string& str);

bool IsDigits(const std::string& str);

/**
 * Trims the whitespace from the left and right sides of the provided string.
 * @param str A pointer to the string to trim.
 */
void TrimString(std::string* str);

/**
 * Converts the provided string to lowercase.
 * @param str A pointer to the string to convert to lowercase.
 */
void LowerString(std::string* str);

/**
 * Creates a friendly time string for a given system clock time point.
 * @param  time The time point to create a time string for.
 * @return      The friendly time string.
 */
std::string FriendlyTime(std::chrono::system_clock::time_point time);
}  // namespace common
}  // namespace mjohnson
