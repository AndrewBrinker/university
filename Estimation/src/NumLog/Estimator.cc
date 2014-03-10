/**
 * Name:       Andrew Brinker
 * Class:      CSE 455, Software Engineering (Winter 2014)
 * School:     California State University, San Bernardino
 *
 * Copyright 2014 Andrew Brinker
 */

#include "./Estimator.h"
#include <string>
#include <cmath>
#include "./NumberLog.h"

#define PERCENTILE_COUNT 2
#define T_TABLE_SIZE 13

static double t_table[PERCENTILE_COUNT][T_TABLE_SIZE] =
{   {   1.963, 1.386, 1.250, 1.190, 1.156, 1.134, 1.119,
        1.108, 1.100, 1.093, 1.074, 1.064, 1.055    },
    {   6.314, 2.920, 2.353, 2.132, 2.015, 1.943, 1.895,
        1.860, 1.833, 1.812, 1.753, 1.725, 1.697    }   };

static int t_dof[T_TABLE_SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20, 30};


double safe_sqrt(double value) {
    if (value < 0.0) return 0.0;
    return sqrt(value);
}


Estimator::Estimator(double estimate,
                     std::string x_file_name,
                     std::string y_file_name) {
    _xk = estimate;
    loadData(x_file_name, y_file_name);
    calculateEstimate();
}


void Estimator::calculateEstimate() {
    getRegressionCoefficients();
    getStandardDeviation();
    getTValue();
    getRange();
    getPredictionIntervals();
}


void Estimator::printResults() {
    printf("70%%\n");
    printf("  T-Value: %f\n", _t_seventy);
    printf("  Range:   %f\n", _range_seventy);
    printf("  UPI:     %f\n", _upi_seventy);
    printf("  LPI:     %f\n", _lpi_seventy);
    printf("90%%\n");
    printf("  T-Value: %f\n", _t_ninety);
    printf("  Range:   %f\n", _range_ninety);
    printf("  UPI:     %f\n", _upi_ninety);
    printf("  LPI:     %f\n", _lpi_ninety);
}


void Estimator::loadData(std::string x_file_name, std::string y_file_name) {
    NumberLog x_log(x_file_name);
    NumberLog y_log(y_file_name);
    auto x_data = x_log.getData();
    auto y_data = y_log.getData();
    auto x_it   = x_data.begin();
    auto y_it   = y_data.begin();
    for (; x_it != x_data.end() && y_it != y_data.end(); ++x_it, ++y_it) {
        data.push_back({*x_it, *y_it});
    }
    _n = data.size();
}


void Estimator::getRegressionCoefficients() {
    _x_avg = 0.0;
    _y_avg = 0.0;
    double num_sum   = 0.0;
    double denom_sum = 0.0;
    double num       = 0.0;
    double denom     = 0.0;
    for (auto pair : data) {
        num_sum   += pair.first * pair.second;
        denom_sum += pair.first * pair.first;
        _x_avg    += pair.first;
        _y_avg    += pair.second;
    }
    _x_avg /= _n;
    _y_avg /= _n;
    num     = (num_sum - (_n * _x_avg * _y_avg));
    denom   = (denom_sum - (_n * (_x_avg * _x_avg)));
    _beta_1 = num / denom;
    _beta_0 = _y_avg - (_beta_1 * _x_avg);
    _yk     = _beta_0 + (_beta_1 * _xk);
}


void Estimator::getStandardDeviation() {
    double sum = 0.0;
    double acc = 0.0;
    for (auto pair : data) {
        acc = pair.second - _beta_0 - (_beta_1 * pair.first);
        sum += (acc * acc);
    }
    sum /= (_n - 2);
    _std_dev = safe_sqrt(sum);
}


void Estimator::getTValue() {
    int dof   = _n - 2;
    int index = -1;
    for (int i = 0; i < PERCENTILE_COUNT; ++i) {
        if (dof == t_dof[i]) {
            index = i;
        }
    }
    if (index == -1) {
        _t_seventy = nan(" ");
        _t_ninety  = nan(" ");
        return;
    }
    _t_seventy = t_table[0][index];
    _t_ninety  = t_table[1][index];
    return;
}


void Estimator::getRange() {
    double num = (_xk - _x_avg) * (_xk - _x_avg);
    double denom = 0.0;
    for (auto pair : data) {
        denom += (pair.first - _x_avg) * (pair.first - _x_avg);
    }
    double square_root = safe_sqrt(1 + (1 / _n) + (num / denom));
    _range_seventy = _t_seventy * _std_dev * square_root;
    _range_ninety = _t_ninety * _std_dev * square_root;
    return;
}


void Estimator::getPredictionIntervals() {
    _upi_seventy = _yk + _range_seventy;
    _upi_ninety  = _yk + _range_ninety;
    _lpi_seventy = _yk - _range_seventy;
    _lpi_ninety  = _yk - _range_ninety;
    return;
}
