/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include <vector>
#include <string>

#ifndef DIR_H
#define DIR_H

struct DirEntry {
  std::string root;
  std::vector<std::string> files;
  std::vector<std::string> subdirectories;
};

std::string join(std::string path, std::string base);
DirEntry dir(std::string root = ".", bool hidden = false);
std::vector<DirEntry> walk(std::string root = ".", bool hidden = false);

#endif  // DIR_H
