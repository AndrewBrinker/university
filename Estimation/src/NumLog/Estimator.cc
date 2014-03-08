/**
 * Name:       Andrew Brinker
 * Class:      CSE 455, Software Engineering (Winter 2014)
 * School:     California State University, San Bernardino
 *
 * Copyright 2014 Andrew Brinker
 */

#include "./Estimator.h"
#include <string>
#include <utility>
#include "./NumberLog.h"

Estimator::Estimator(std::string x_file_name, std::string y_file_name) {
    NumberLog x_log(x_file_name);
    NumberLog y_log(y_file_name);
    auto x_data = x_log.getData();
    auto y_data = y_log.getData();
    auto x_it   = x_data.begin();
    auto y_it   = y_data.begin();
    for (; x_it != x_data.end() && y_it != y_data.end(); ++x_it, ++y_it) {
        data.push_back({*x_it, *y_it});
    }
    calculate();
}


void Estimator::calculate() {
    int    n         = data.size();
    double num_sum   = 0.0;
    double denom_sum = 0.0;
    double x_avg     = 0.0;
    double y_avg     = 0.0;
    double num       = 0.0;
    double denom     = 0.0;

    for (auto pair : data) {
        num_sum   += pair.first * pair.second;
        denom_sum += pair.first * pair.first;
        x_avg     += pair.first;
        y_avg     += pair.second;
    }

    x_avg   /= n;
    y_avg   /= n;

    num     = (num_sum - (n * x_avg * y_avg));
    denom   = (denom_sum - (n * (x_avg * x_avg)));

    _beta_1 = num / denom;
    _beta_0 = y_avg - (_beta_1 * x_avg);

    printf("Beta 1: %f\n", _beta_1);
    printf("Beta 0: %f\n", _beta_0);
    printf("Regression line: y = %f + (%f * x)\n", _beta_0, _beta_1);
}
