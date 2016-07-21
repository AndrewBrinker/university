/**
 * Name:       Andrew Brinker
 * Class:      CSE 455, Software Engineering (Winter 2014)
 * School:     California State University, San Bernardino
 *
 * Copyright 2014 Andrew Brinker
 */

#include <NumLog/Estimator.h>
#include <string>
#include <iostream>
#include <cstdlib>

int main(int argc, char **argv) {
  if (argc != 4) {
    printf("Usage: %s <Estimated LOC> <X-Axis File> <Y-Axis File>\n", argv[0]);
    return 0;
  }
  Estimator e(atof(argv[1]), argv[2], argv[3]);
  e.printResults();
  return 0;
}
