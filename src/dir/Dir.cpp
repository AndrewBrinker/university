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
  if (path.back() == '/')
    path.pop_back();
  return path + "/" + base;
}

DirEntry dir(std::string root, bool hidden) {
  DIR* currDir;
  dirent* currDirEnt;
  DirEntry de;
  std::string currFilePath;
  struct stat statbuf;

  de.root = root;

  // Try to open the current root directory
  if ((currDir = opendir(root.c_str()))) {
    // Iterate over contents
    while ((currDirEnt = readdir(currDir))) {
      // If we're skipping hidden files, then skip them.
      if (!hidden && currDirEnt->d_name[0] == '.')
        continue;

      // Skip special subdirectories
      if (strcmp(currDirEnt->d_name, "." ) == 0 ||
          strcmp(currDirEnt->d_name, "..") == 0)
        continue;

      // Get path of file in question
      currFilePath = join(root, currDirEnt->d_name);

      stat(currFilePath.c_str(), &statbuf);

      // Regular files
      if (S_ISREG(statbuf.st_mode))
        de.files.push_back(currFilePath);

      // Subdirectories
      else if (S_ISDIR(statbuf.st_mode))
        de.subdirectories.push_back(currFilePath);
    }
  }

  std::sort(de.files.begin(), de.files.end());
  std::sort(de.subdirectories.begin(), de.subdirectories.end());

  return de;
}

std::vector<DirEntry> walk(std::string root, bool hidden) {
  std::vector<DirEntry> dirListing;
  DirEntry currDirEntry;
  std::string currFilePath;

  std::stack<std::string> subdirStack;
  subdirStack.push(root);

  while (!subdirStack.empty()) {
    // Get next subdirectory
    root = subdirStack.top();
    subdirStack.pop();

    currDirEntry = dir(root, hidden);

    // Push subdirectories onto stack
    // We push in reverse order to preserve sortedness
    for (std::vector<std::string>::reverse_iterator
         rit = currDirEntry.subdirectories.rbegin();
         rit != currDirEntry.subdirectories.rend();
         ++rit)
      subdirStack.push(*rit);

    dirListing.push_back(currDirEntry);
  }

  return dirListing;
}
