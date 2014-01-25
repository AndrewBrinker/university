/**
 * Name:       Andrew Brinker
 * Class:      CSE 455, Software Engineering (Winter 2014)
 * School:     California State University, San Bernardino
 * Assignment: 2A
 *
 * Copyright 2014 Andrew Brinker
 */

#include <LineCount/LineCount.h>
#include <iostream>
#include <string>

int main() {
  std::string file_name;
  std::cout << "Filename: ";
  std::cin >> file_name;

  // Scan the file with the given name, and provide the appropriate response.
  lc::count << file_name;
  if (lc::count.good()) {
    std::cout << "Lines of code: " << lc::count.loc() << std::endl;
  } else {
    std::cout << "Input failed. Please try again." << std::endl;
  }

  return 0;
}
