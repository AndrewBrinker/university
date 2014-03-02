/**
 * Name:       Andrew Brinker
 * Class:      CSE 455, Software Engineering (Winter 2014)
 * School:     California State University, San Bernardino
 *
 * Copyright 2014 Andrew Brinker
 */

#include <string>

#ifndef REGRESSION_H
#define REGRESSION_H

class Regression {
 public:
  explicit Regression(std::string);

 private:
    int _beta_0;
    int _beta_1;
};

#endif  // REGRESSION_H
