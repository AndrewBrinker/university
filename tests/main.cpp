/*
 * Copyright 2013 Andrew Brinker
 */

// This is the central testing program.

#include <iostream>
#include <string>
#include "VdiskTest.cpp"
#include "FileSysTest.cpp"
#include "TableTest.cpp"

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
