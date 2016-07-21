/**
 * Name:       Andrew Brinker
 * Class:      CSE 455, Software Engineering (Winter 2014)
 * School:     California State University, San Bernardino
 * Assignment: 2A
 *
 * Copyright 2014 Andrew Brinker
 */

#include <ObjectCount/ObjectCount.h>
#include <iostream>
#include <string>


/*
 * Pass in name of file w/out extension
 * Searches through both .h and .cc
 * The sum of the two line counts is object LOC
 * The number of methods is the line count of the .h file - 1.
 */

int main() {
  std::string object_name;
  std::cout << "Object name: ";
  std::cin >> object_name;

  // Scan the file with the given name, and provide the appropriate response.
  lc::count << object_name;
  if (lc::count.good()) {
    std::cout << "Lines of code: " << lc::count.loc() << std::endl;
    std::cout << "# of Methods: " << lc::count.methods() << std::endl;
  } else {
    std::cout << "Input failed. Please try again." << std::endl;
  }

  return 0;
}
