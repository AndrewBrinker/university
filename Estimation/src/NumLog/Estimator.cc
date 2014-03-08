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
{
    {   1.963, 1.386, 1.250, 1.190, 1.156, 1.134, 1.119,
        1.108, 1.100, 1.093, 1.074, 1.064, 1.055    },
    {   6.314, 2.920, 2.353, 2.132, 2.015, 1.943, 1.895,
        1.860, 1.833, 1.812, 1.753, 1.725, 1.697    }
};

static int t_dof[T_TABLE_SIZE] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20, 30
};


Estimator::Estimator(int estimate, std::string x_file_name, std::string y_file_name) {
    _xk = estimate;
    loadData(x_file_name, y_file_name);
    calculateEstimate();
}


void Estimator::calculateEstimate() {
    getRegressionCoefficients();
    getStandardDeviation();
    getTValue();
    getRange();
}


void Estimator::printResults() {
    printf("Beta 0:\n\t%f\n", _beta_0);
    printf("Beta 1:\n\t%f\n", _beta_1);
    printf("Regression Line:\n\ty = %f + (%f * x)\n", _beta_0, _beta_1);
    printf("Standard Deviation:\n\t%f\n", _std_dev);
    printf("T-Value (70%%):\n\t%f\n", _t_seventy);
    printf("T-Value (90%%):\n\t%f\n", _t_ninety);
    printf("Range (70%%):\n\t%f\n", _range_seventy);
    printf("Range (90%%):\n\t%f\n", _range_ninety);
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
}


void Estimator::getRegressionCoefficients() {
    _n     = data.size();
    _x_avg = 0.0;
    _y_avg = 0.0;
    double num_sum   = 0.0;
    double denom_sum = 0.0;
    double num       = 0.0;
    double denom     = 0.0;
    for (auto pair : data) {
        num_sum   += pair.first * pair.second;
        denom_sum += pair.first * pair.first;
        _x_avg     += pair.first;
        _y_avg     += pair.second;
    }
    _x_avg   /= _n;
    _y_avg   /= _n;
    num     = (num_sum - (_n * _x_avg * _y_avg));
    denom   = (denom_sum - (_n * (_x_avg * _x_avg)));
    _beta_1 = num / denom;
    _beta_0 = _y_avg - (_beta_1 * _x_avg);
}


void Estimator::getStandardDeviation() {
    double sum = 0.0;
    double placeholder = 0.0;
    for (auto pair : data) {
        placeholder = pair.second - _beta_0 - (_beta_1 * pair.first);
        sum += (placeholder * placeholder);
    }
    sum /= (_n - 2);
    _std_dev = sqrt(sum);
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
    double square_root = sqrt(1 + (1 / _n) + (num / denom));
    _range_seventy = _t_seventy * _std_dev * square_root;
    _range_ninety = _t_ninety * _std_dev * square_root;
    return;
}

