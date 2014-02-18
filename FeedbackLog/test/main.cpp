/**
 * Name:       Andrew Brinker
 * Class:      CSE 455, Software Engineering (Winter 2014)
 * School:     California State University, San Bernardino
 * Assignment: 2B
 *
 * Copyright 2014 Andrew Brinker
 */

#include <NumLog/FeedbackLog.h>
#include <string>
#include <iostream>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << " <file name>" << std::endl;
    return 0;
  }
  FeedbackLog session(argv[1]);
  return 0;
}
