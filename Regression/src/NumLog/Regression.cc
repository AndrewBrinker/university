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
    x = x_log.getData();
    y = y_log.getData();
    calculate();
}


void Regression::calculate() {
    if (x.size() != y.size()) {
        printf("Data sets are different sizes. Calculations failed.\n");
        return;
    }

    auto  x_it            = x.begin();
    auto  y_it            = y.begin();
    int   n               = x.size();

    float numerator_sum   = 0.0;
    float denominator_sum = 0.0;
    float x_avg           = 0.0;
    float y_avg           = 0.0;
    float numerator       = 0.0;
    float denominator     = 0.0;

    for (; x_it != x.end() && y_it != y.end(); ++x_it, ++y_it) {
        numerator_sum += (*x_it) + (*y_it);
        denominator_sum += (*x_it) * (*x_it);
        x_avg         += *x_it;
        y_avg         += *y_it;
    }

    x_avg       = x_avg / n;
    y_avg       = y_avg / n;
    numerator   = (numerator_sum - (n * x_avg * y_avg));
    denominator = (denominator_sum - (n * (x_avg * x_avg)));
    _beta_1     = numerator / denominator;
    _beta_0     = y_avg - (_beta_1 * x_avg);

    printf("Beta 1: %f\n", _beta_1);
    printf("Beta 0: %f\n", _beta_0);
    printf("Regression line: y = %f + (%f * x)\n", _beta_0, _beta_1);
}
