/**
 * Name:       Andrew Brinker
 * Class:      CSE 455, Software Engineering (Winter 2014)
 * School:     California State University, San Bernardino
 *
 * Copyright 2014 Andrew Brinker
 */

#include <string>
#include <list>
#include <utility>

#ifndef REGRESSION_H
#define REGRESSION_H

class Regression {
 public:
  explicit Regression(std::string, std::string);
  void calculate();

 private:
    std::list<std::pair<float, float>> data;
    double _beta_0;
    double _beta_1;
};

#endif  // REGRESSION_H
