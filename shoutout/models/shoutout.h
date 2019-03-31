// Copyright 2019 Michael Johnson

#pragma once

#include <string>
#include <vector>

#include "shoutout/models/user.h"

namespace mjohnson {
namespace shoutout {
/**
 * Provides core data storage and persistence capabilities.
 */
class ShoutOut {
 private:
  /**
   * Provides a single global instance of ShoutOut for user by any caller.
   */
  static ShoutOut* global_shoutout_;

  /**
   * The list of all users.
   */
  std::vector<User*>* users_;

  /**
   * The top level data directory.
   */
  const std::string data_directory_;

  /**
   * Retrieves a list of user directories from the filesystem.
   *
   * @return A list of user directories, without the data_directory_ prefix.
   */
  std::vector<std::string> GetUserDirs();
  /**
   * Prepares the data directories by creating directories for new users.
   */
  void PrepareDirectories();
  /**
   * Writes a particular user's data to disk.
   * @param user The user whose data should be written to disk.
   */
  void WriteUserToDisk(User* user);

  /**
   * Gets the data directory path for a particular User.
   * @param  user_id The User's ID.
   * @return The path to the user's data directory.
   */
  std::string GetUserPath(const std::string& user_id);

  /**
   * Opens a user data file for reading.
   * @param user_id The User's ID.
   * @param filename The name of the file inside the User's data directory.
   * @return An ifstream pointing to the opened file.
   */
  std::ifstream OpenUserFileRead(const std::string& user_id,
                                 const std::string& filename);

  /**
   * Opens a user data file for writing.
   * @param user_id The User's ID.
   * @param filename The name of the file inside the User's data directory.
   * @return An ofstream pointing to the opened file.
   */
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

  /**
   * Constructs the ShoutOut data manager using a specified data directory.
   * @param data_directory The directory in which to store User data.
   */
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

  /**
   * Writes all User data to disk.
   */
  void WriteToDisk();

  /**
   * Retrieves the list of Users.
   * @return All Users in the ShoutOut system.
   */
  std::vector<User*>* Users() { return this->users_; }

  /**
   * Retrieves a specific user by their user ID.
   * @param user_id The user ID to search for.
   * @return The User corresponding to the given user ID or nullptr if no such
   * user was found.
   */
  User* GetUserById(const std::string& user_id);

  /**
   * Retrieves a specific user by their username.
   * @param username The username to search for.
   * @return The User corresponding to the given username or nullptr if no such
   * user was found.
   */
  User* GetUserByName(const std::string& username);

  /**
   * Adds a new User to the ShoutOut instance.
   * @param user The User to be added.
   */
  void AddUser(User* user);
};

/**
 * An exception thrown when ShoutOut::CreateGlobalShoutOut is called, but has
 * already been called before.
 */
class GlobalShoutOutExistsException : public std::logic_error {
 public:
  /**
   * A constructor that constructs the GlobalShoutOutExistsException with a
   * message of "A global ShoutOut instance already exists".
   */
  GlobalShoutOutExistsException()
      : std::logic_error("A global ShoutOut instance already exists") {}
};
}  // namespace shoutout
}  // namespace mjohnson
