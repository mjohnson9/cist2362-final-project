// Copyright 2019 Michael Johnson

#pragma once

#include <string>
#include <vector>

#include "shoutout/models/user.h"

namespace mjohnson {
namespace shoutout {
/**
 * Provides the main menu. The main menu allows a user to register or login. It
 * can also display an error message from another view.
 */
class ShoutOut {
 private:
  /**
   * Provides a single global instance of ShoutOut for user by any caller.
   */
  static ShoutOut* global_shoutout_;

  std::vector<User*>* users_;

  const std::string data_directory_;

  std::vector<std::string> GetUserDirs();
  void PrepareDirectories();
  void WriteUserToDisk(User* user);

  std::string GetUserPath(const std::string& user_id);

  std::ifstream OpenUserFileRead(const std::string& user_id,
                                 const std::string& filename);

  std::ofstream OpenUserFileWrite(const std::string& user_id,
                                  const std::string& filename);

 public:
  /**
   * Retrieves the global ShoutOut instance.
   * @return The global ShoutOut instance.
   */
  static ShoutOut* Get();

  /**
   * Creates the global ShoutOut instance with the specified data directory. An
   * exception is thrown if there is already a global ShoutOut instance.
   *
   * @param  data_directory The directory to be used for data storage.
   * @return                The new global ShoutOut instance.
   */
  static ShoutOut* CreateGlobalShoutOut(const std::string& data_directory);

  explicit ShoutOut(std::string data_directory);

  /**
   * Retrieves the data directory.
   * @return The data directory's path.
   */
  std::string DataDirectory() const { return this->data_directory_; }

  /**
   * Attempts to load the ShoutOut information from the disk. If the directory
   * doesn't exist, it silently fails. If there is corruption in the file
   * hierarchy, it throws an exception.
   */
  void LoadFromDisk();

  void WriteToDisk();

  std::vector<User*>* Users();

  /**
   * Retrieves a specific user by their user ID.
   * @param  user_id The user ID to search for.
   * @return         The User corresponding to the given user ID or nullptr if
   *                 no such user was found.
   */
  User* GetUserById(const std::string& user_id);

  /**
   * Retrieves a specific user by their username.
   * @param  user_id The username to search for.
   * @return         The User corresponding to the given username or nullptr if
   *                 no such user was found.
   */
  User* GetUserByName(const std::string& username);

  /**
   * Adds a new User to the ShoutOut instance.
   * @param user
   */
  void AddUser(User* user);
};

class GlobalShoutOutExistsException : public std::logic_error {
 public:
  GlobalShoutOutExistsException()
      : std::logic_error("A global ShoutOut instance already exists") {}
};
}  // namespace shoutout
}  // namespace mjohnson
