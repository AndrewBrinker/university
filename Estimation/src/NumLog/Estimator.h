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
  explicit Estimator(int, std::string, std::string);
  void calculateEstimate();
  void printResults();

 private:
  void loadData(std::string, std::string);
  void getRegressionCoefficients();
  void getStandardDeviation();
  void getTValue();
  void getRange();

  std::list<std::pair<float, float>> data;

  int    _n;
  int    _xk;
  double _x_avg;
  double _y_avg;
  double _beta_0;
  double _beta_1;
  double _std_dev;
  double _t_seventy;
  double _t_ninety;
  double _range_seventy;
  double _range_ninety;
};

#endif  // ESTIMATOR_H
