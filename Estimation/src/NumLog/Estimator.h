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
  explicit Estimator(double, std::string, std::string);
  void calculateEstimate();
  void printResults();

 private:
  void loadData(std::string, std::string);
  void getRegressionCoefficients();
  void getStandardDeviation();
  void getTValue();
  void getRange();
  void getPredictionIntervals();

  std::list<std::pair<float, float>> data;

  int    _n;

  double _xk;
  double _yk;

  double _x_avg;
  double _y_avg;

  double _beta_0;
  double _beta_1;
  double _std_dev;

  double _t_seventy;
  double _range_seventy;
  double _upi_seventy;
  double _lpi_seventy;

  double _t_ninety;
  double _range_ninety;
  double _upi_ninety;
  double _lpi_ninety;
};

#endif  // ESTIMATOR_H
