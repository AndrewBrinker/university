/*
 * Copyright 2013 Andrew Brinker
 */

// This is the central testing program.

#include <iostream>
#include <string>
#include "Vdisk_tests.cpp"
#include "FileSys_tests.cpp"
#include "Table_tests.cpp"

int main() {
  std::string buffer = "";
  if (!virtualDiskTest(buffer)) {
    std::cout << "VirtualDisk testing failed with the error:\n    "
              << buffer << std::endl;
    return 0;
  }
  if (!fileSystemTest(buffer)) {
    std::cout << "FileSystem testing failed with the error:\n    "
              << buffer << std::endl;
    return 0;
  }
  if (!tableTest(buffer)) {
    std::cout << "Table testing failed with the error:\n    "
              << buffer << std::endl;
    return 0;
  }
  std::cout << "All tests passed." << std::endl;
  return 1;
}
