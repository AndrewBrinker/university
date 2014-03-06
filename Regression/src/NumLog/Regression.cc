/**
 * Name:       Andrew Brinker
 * Class:      CSE 455, Software Engineering (Winter 2014)
 * School:     California State University, San Bernardino
 *
 * Copyright 2014 Andrew Brinker
 */

#include "./Regression.h"
#include <string>
#include "./NumberLog.h"

Regression::Regression(std::string x_file_name, std::string y_file_name) {
    NumberLog x_log(x_file_name);
    NumberLog y_log(y_file_name);
    for (auto item : x_log.getData()) x.push_back(item);
    for (auto item : y_log.getData()) y.push_back(item);
    calculate();
}


void Regression::calculate() {
    if (x.size() != y.size()) {
        printf("Data sets are different sizes. Calculations failed.\n");
        return;
    }

    int    n         = x.size();
    double num_sum   = 0.0;
    double denom_sum = 0.0;
    double x_avg     = 0.0;
    double y_avg     = 0.0;
    double num       = 0.0;
    double denom     = 0.0;

    for (int i = 0; i < n; ++i) {
        num_sum   += x[i] * y[i];
        denom_sum += x[i] * x[i];
        x_avg     += x[i];
        y_avg     += y[i];
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
