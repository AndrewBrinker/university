/**
 * Name:       Andrew Brinker
 * Class:      CSE 455, Software Engineering (Winter 2014)
 * School:     California State University, San Bernardino
 *
 * Copyright 2014 Andrew Brinker
 */

#include <NumLog/Regression.h>
#include <string>
#include <iostream>

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Usage: %s <file name>\n", argv[0]);
    return 0;
  }
  Regression session(argv[1], argv[2]);
  return 0;
}
