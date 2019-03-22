// Copyright 2019 Michael Johnson

#include "shoutout/models/shoutout.h"

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <algorithm>
#include <cerrno>
#include <chrono>  // NOLINT(build/c++11)
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>

#include "shoutout/common.h"
#include "shoutout/exceptions.h"
#include "shoutout/models/shout.h"

namespace mjohnson {
namespace shoutout {
ShoutOut* ShoutOut::global_shoutout_ = nullptr;

ShoutOut::ShoutOut(std::string data_directory)
    : data_directory_(std::move(data_directory)) {}

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

  std::vector<std::string> user_dirs = this->GetUserDirs();

  for (const std::string& user_id : user_dirs) {
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

  for (User* user : *this->users_) {
    const std::string user_id = user->UserId();

    {
      // Read following.txt to find out who the user is following
      std::ifstream following_file =
          this->OpenUserFileRead(user_id, "following.txt");

      while (!following_file.eof()) {
        if (following_file.fail()) {
          following_file.close();
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
          following_file.close();
          throw DataCorruptedException(user->Username() +
                                       " is following themself");
        }

        User* other_user = this->GetUserById(following_id);
        if (other_user == nullptr) {
          following_file.close();
          throw DataCorruptedException(
              user->Username() +
              " is following non-existant user: " + following_id);
        }

        user->AddFollowing(other_user);
        other_user->AddFollower(user);
      }

      following_file.close();
    }

    {
      std::ifstream shouts_file = this->OpenUserFileRead(user_id, "shouts.txt");

      while (!shouts_file.eof()) {
        if (shouts_file.fail()) {
          shouts_file.close();
          throw DataCorruptedException(
              "Failed to read " + user->UserId() +
              "'s shouts file: " + std::string(strerror(errno)));
        }

        std::string line;
        std::getline(shouts_file, line);
        if (line.empty()) {
          // We read a blank line; ignore it
          continue;
        }

        std::stringstream linestream(line);

        uint64_t seconds_since_epoch_int;
        std::string content;
        linestream >> seconds_since_epoch_int;
        std::getline(linestream, content);
        mjohnson::common::TrimString(&content);

        std::chrono::seconds seconds_since_epoch(seconds_since_epoch_int);
        std::chrono::system_clock::time_point shout_time(seconds_since_epoch);

        Shout* new_shout = new Shout(user, content, shout_time);
        user->AddShout(new_shout);
      }

      shouts_file.close();
    }
  }
}

void ShoutOut::WriteToDisk() {
  {
    auto status = mkdir(this->data_directory_.c_str(),
                        S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
    if (status != 0) {
      if (errno != EEXIST) {
        throw DataCorruptedException("Failed to create " +
                                     this->data_directory_ + ": " +
                                     std::string(strerror(errno)));
      }
    }
  }
  this->PrepareDirectories();

  for (User* user : *this->users_) {
    this->WriteUserToDisk(user);
  }
}

void ShoutOut::WriteUserToDisk(User* user) {
  const std::string user_id = user->UserId();

  {
    std::ofstream username_file =
        this->OpenUserFileWrite(user_id, "username.txt");

    if (!(username_file << user->Username())) {
      username_file.close();
      throw DataCorruptedException(
          "Failed to write to " + user_id +
          "'s username file: " + std::string(strerror(errno)));
    }

    username_file.close();
  }

  {
    std::ofstream password_file =
        this->OpenUserFileWrite(user_id, "password.txt");

    if (!(password_file << user->Password())) {
      password_file.close();
      throw DataCorruptedException(
          "Failed to write to " + user_id +
          "'s password file: " + std::string(strerror(errno)));
    }

    password_file.close();
  }

  {
    std::ofstream following_file =
        this->OpenUserFileWrite(user_id, "following.txt");

    for (User* following : *user->Following()) {
      if (!(following_file << following->Username() << std::endl)) {
        following_file.close();
        throw DataCorruptedException(
            "Failed to write to " + user_id +
            "'s following file: " + std::string(strerror(errno)));
      }
    }

    following_file.close();
  }

  {
    std::ofstream shouts_file = this->OpenUserFileWrite(user_id, "shouts.txt");

    for (Shout* shout : *user->Shouts()) {
      auto duration_since_epoch =
          std::chrono::duration_cast<std::chrono::seconds>(
              shout->Time().time_since_epoch());
      uint64_t seconds_since_epoch = duration_since_epoch.count();

      if (!(shouts_file << seconds_since_epoch << shout->Content()
                        << std::endl)) {
        shouts_file.close();
        throw DataCorruptedException(
            "Failed to write to " + user_id +
            "'s shouts file: " + std::string(strerror(errno)));
      }
    }

    shouts_file.close();
  }
}

void ShoutOut::PrepareDirectories() {
  std::vector<std::string> user_dirs = this->GetUserDirs();

  std::vector<std::string> user_ids;
  for (User* user : *this->users_) {
    user_ids.push_back(user->UserId());
  }

  const size_t difference_size =
      user_dirs.size() > user_ids.size() ? user_dirs.size() : user_ids.size();
  std::vector<std::string> new_users(difference_size);

  // Before calculating the difference, both vectors must be sorted
  std::sort(user_dirs.begin(), user_dirs.end());
  std::sort(user_ids.begin(), user_ids.end());

  // Get the new users
  auto output_iterator =
      set_difference(user_ids.begin(), user_ids.end(), user_dirs.begin(),
                     user_dirs.end(), new_users.begin());
  new_users.resize(output_iterator - new_users.begin());

  for (const std::string& user_id : new_users) {
    mkdir((this->data_directory_ + "/" + user_id).c_str(),
          S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
  }
}

std::vector<std::string> ShoutOut::GetUserDirs() {
  DIR* outer_dir = opendir(this->data_directory_.c_str());
  if (outer_dir == nullptr) {
    if (errno == ENOENT) {
      return std::vector<std::string>();
    }

    throw DataCorruptedException("Failed to open " + this->data_directory_ +
                                 ": " + std::string(strerror(errno)));
  }

  std::vector<std::string> dirs;

  for (dirent* dir_entry = readdir(outer_dir); dir_entry != nullptr;
       dir_entry = readdir(outer_dir)) {
    std::string dir(dir_entry->d_name);
    if (dir == "." || dir == "..") {
      // Ignore entries for current directory and parent directory
      continue;
    }

    if (dir_entry->d_type != DT_DIR) {
      closedir(outer_dir);
      throw DataCorruptedException(
          "Found non-directory in top-level data directory");
    }

    dirs.push_back(dir);
  }

  closedir(outer_dir);

  return dirs;
}

User* ShoutOut::GetUserById(const std::string& user_id) {
  std::string desired_user_id = User::CreateUserId(user_id);
  for (User* user : *this->users_) {
    if (user->UserId() == desired_user_id) {
      return user;  // This is a match
    }
  }

  return nullptr;  // The user doesn't exist
}

User* ShoutOut::GetUserByName(const std::string& username) {
  std::string desired_username(username);
  mjohnson::common::LowerString(&desired_username);

  for (User* user : *this->users_) {
    std::string user_username(user->Username());
    mjohnson::common::LowerString(&user_username);
    if (user_username == desired_username) {
      return user;  // This is a match
    }
  }

  return nullptr;  // The user doesn't exist
}

void ShoutOut::AddUser(User* user) { this->users_->push_back(user); }

std::string ShoutOut::GetUserPath(const std::string& user_id) {
  return this->data_directory_ + "/" + user_id;
}

std::ofstream ShoutOut::OpenUserFileWrite(const std::string& user_id,
                                          const std::string& filename) {
  const std::string path = this->GetUserPath(user_id) + "/" + filename;

  std::ofstream file(path, std::ios_base::out | std::ios_base::trunc);
  if (!file.good()) {
    throw DataCorruptedException("Failed to open " + path + " for writing: " +
                                 std::string(strerror(errno)));
  }

  return file;
}

std::ifstream ShoutOut::OpenUserFileRead(const std::string& user_id,
                                         const std::string& filename) {
  const std::string path = this->GetUserPath(user_id) + "/" + filename;

  std::ifstream file(path, std::ifstream::in);
  if (!file.good()) {
    throw DataCorruptedException("Failed to open " + path + " for reading: " +
                                 std::string(strerror(errno)));
  }

  return file;
}
}  // namespace shoutout
}  // namespace mjohnson
