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

#ifndef ESTIMATOR_H
#define ESTIMATOR_H

class Estimator {
 public:
  explicit Estimator(std::string, std::string);
  void calculateEstimate();
  void printResults();

 private:
  void loadData(std::string, std::string);
  void getRegressionCoefficients();

  std::list<std::pair<float, float>> data;

  double _beta_0;
  double _beta_1;
};

#endif  // ESTIMATOR_H
