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
  void getCorrelation();
  void getSignificance();

  std::list<std::pair<float, float>> data;

  int    _n;

  double _r;
  double _s;

  friend void exit_with_error(int code, std::string msg);
  friend double safe_sqrt(double value);
  friend double square(double value);
};

#endif  // ESTIMATOR_H
