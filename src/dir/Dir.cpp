/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include "./Dir.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <algorithm>
#include <stack>
#include <cstring>
#include <vector>
#include <string>

std::string join(std::string path, std::string base) {
  if (path.back() == '/') {
    path.pop_back();
  }
  return path + "/" + base;
}

DirEntry dir(std::string root, bool hidden) {
  DIR* current_directory;
  dirent* current_directory_entry;
  DirEntry de;
  std::string current_file_path;
  struct stat statbuf;

  de.root = root;

  // Try to open the current root directory
  if ((current_directory = opendir(root.c_str()))) {
    // Iterate over contents
    while ((current_directory_entry = readdir(current_directory))) {  // NOLINT
      // If we're skipping hidden files, then skip them.
      if (!hidden && current_directory_entry->d_name[0] == '.') {
        continue;
      }

      // Skip special subdirectories
      if (strcmp(current_directory_entry->d_name, "." ) == 0 ||
          strcmp(current_directory_entry->d_name, "..") == 0) {
        continue;
      }

      // Get path of file in question
      current_file_path = join(root, current_directory_entry->d_name);
      stat(current_file_path.c_str(), &statbuf);

      if (S_ISREG(statbuf.st_mode)) {
        // Regular file
        de.files.push_back(current_file_path);
      } else if (S_ISDIR(statbuf.st_mode)) {
        // Subdirectory
        de.subdirectories.push_back(current_file_path);
      }
    }
  }

  std::sort(de.files.begin(), de.files.end());
  std::sort(de.subdirectories.begin(), de.subdirectories.end());

  return de;
}

std::vector<DirEntry> walk(std::string root, bool hidden) {
  std::vector<DirEntry> directory_listing;
  DirEntry current_directory_entry;
  std::string current_file_path;

  std::stack<std::string> subdirectory_stack;
  subdirectory_stack.push(root);

  while (!subdirectory_stack.empty()) {
    // Get next subdirectory
    root = subdirectory_stack.top();
    subdirectory_stack.pop();
    current_directory_entry = dir(root, hidden);

    // Push subdirectories onto stack
    // We push in reverse order to preserve sortedness
    for (std::vector<std::string>::reverse_iterator
         rit = current_directory_entry.subdirectories.rbegin();
         rit != current_directory_entry.subdirectories.rend();
         ++rit) {
      subdirectory_stack.push(*rit);
    }

    directory_listing.push_back(current_directory_entry);
  }

  return directory_listing;
}
