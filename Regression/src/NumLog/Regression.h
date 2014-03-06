/**
 * Name:       Andrew Brinker
 * Class:      CSE 455, Software Engineering (Winter 2014)
 * School:     California State University, San Bernardino
 *
 * Copyright 2014 Andrew Brinker
 */

#include <string>
#include <vector>

#ifndef REGRESSION_H
#define REGRESSION_H

class Regression {
 public:
  explicit Regression(std::string, std::string);
  void calculate();

 private:
    std::vector<double> x;
    std::vector<double> y;
    double _beta_0;
    double _beta_1;
};

#endif  // REGRESSION_H
