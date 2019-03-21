// Copyright 2019 Michael Johnson

#include "shoutout/models/shoutout.h"

#include <dirent.h>
#include <errno.h>
#include <fstream>
#include <iostream>

#include "shoutout/exceptions.h"

namespace mjohnson {
namespace shoutout {
ShoutOut* ShoutOut::global_shoutout_ = nullptr;

ShoutOut::ShoutOut(const std::string& data_directory)
    : data_directory_(data_directory) {}

ShoutOut* ShoutOut::Get() { return ShoutOut::global_shoutout_; }

ShoutOut* ShoutOut::CreateGlobalShoutOut(const std::string& data_directory) {
  if (ShoutOut::global_shoutout_ != nullptr) {
    throw GlobalShoutOutExistsException();
  }

  ShoutOut::global_shoutout_ = new ShoutOut(data_directory);
  return ShoutOut::global_shoutout_;
}

void ShoutOut::LoadFromDisk() {
  this->users_ = new std::vector<User*>();

  DIR* outer_dir = opendir(this->data_directory_.c_str());
  if (outer_dir == NULL) {
    if (errno == ENOENT) {  // The directory doesn't exist
      return;
    }
    throw DataCorruptedException("Failed to open " + this->data_directory_ +
                                 ": " + std::string(strerror(errno)));
  }

  for (dirent* dir_entry = readdir(outer_dir); dir_entry != nullptr;
       dir_entry = readdir(outer_dir)) {
    std::string user_id(dir_entry->d_name);
    if (user_id == "." || user_id == "..") {
      // Ignore entries for current directory and parent directory
      continue;
    }

    std::string username;
    std::string password;

    {
      std::ifstream username_file =
          this->OpenUserFileRead(user_id, "username.txt");

      std::getline(username_file, username);  // Read the username
      if (!username_file) {                   // The read failed
        username_file.close();  // Close the file before doing anything else

        throw DataCorruptedException(
            "Failed to read " + user_id +
            "'s username: " + std::string(strerror(errno)));
      }

      username_file.close();  // Close the file now that we're done with it
    }

    {
      std::ifstream password_file =
          this->OpenUserFileRead(user_id, "password.txt");

      std::getline(password_file, password);  // Read the password
      if (!password_file) {                   // The read failed
        password_file.close();  // Close the file before doing anything else

        throw DataCorruptedException(
            "Failed to read " + user_id +
            "'s password: " + std::string(strerror(errno)));
      }

      password_file.close();  // Close the file now that we're done with it
    }

    User* user = new User(username, password);
    this->users_->push_back(user);
  }

  closedir(outer_dir);

  for (User* user : *this->users_) {
    const std::string user_id = user->UserId();
    std::ifstream following_file =
        this->OpenUserFileRead(user_id, "following.txt");

    while (!following_file.eof()) {
      if (following_file.fail()) {
        throw DataCorruptedException(
            "Failed to read " + user->UserId() +
            "'s following file: " + std::string(strerror(errno)));
      }

      std::string following_id;
      following_file >> following_id;

      if (following_id.empty()) {
        // We read a blank line; ignore it
        continue;
      }

      if (following_id == user_id) {
        throw DataCorruptedException(user->Username() +
                                     " is following themself");
      }

      User* other_user = this->GetUser(following_id);
      if (other_user == nullptr) {
        throw DataCorruptedException(
            user->Username() +
            " is following non-existant user: " + following_id);
      }

      user->AddFollowing(other_user);
      other_user->AddFollower(user);
    }
  }
}

User* ShoutOut::GetUser(const std::string& user_id) {
  for (User* user : *this->users_) {
    if (user->UserId() == user_id) {
      return user;  // This is a match
    }
  }

  return nullptr;  // The user doesn't exist
}

std::ifstream ShoutOut::OpenUserFileRead(const std::string& user_id,
                                         const std::string& filename) {
  const std::string path =
      this->data_directory_ + "/" + user_id + "/" + filename;

  std::ifstream file(path, std::ifstream::in);
  if (!file.good()) {
    throw DataCorruptedException("Failed to open " + path + ": " +
                                 std::string(strerror(errno)));
  }

  return file;
}
}  // namespace shoutout
}  // namespace mjohnson
