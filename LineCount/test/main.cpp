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

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << " <file name>" << std::endl;
    return 0;
  }
  int total = 0;
  for (int i = 1; i < argc; ++i) {
    std::cout << "Filename: " << argv[i] << std::endl;
    lc::count << argv[i];
    if (lc::count.good()) {
      std::cout << "\tLines of code: " << lc::count.loc() << std::endl;
      total += lc::count.loc();
    } else {
      std::cout << "\tInput failed. Please try again." << std::endl;
    }
    lc::count.clear();
  }
  std::cout << "Total: " << total << std::endl;
  return 0;
}
