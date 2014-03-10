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
#include <cstdlib>
#include "./NumberLog.h"

#define PERCENTILE_COUNT 2
#define T_TABLE_SIZE 13

static double t_table[PERCENTILE_COUNT][T_TABLE_SIZE] =
{   {   1.963, 1.386, 1.250, 1.190, 1.156, 1.134, 1.119,
        1.108, 1.100, 1.093, 1.074, 1.064, 1.055    },
    {   6.314, 2.920, 2.353, 2.132, 2.015, 1.943, 1.895,
        1.860, 1.833, 1.812, 1.753, 1.725, 1.697    }   };

static int t_dof[T_TABLE_SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20, 30};


/**
 * Get the square root of a number, and quit loudly if the number is negative.
 * @param  value -> The value being square-rooted.
 * @return the square root of the value.
 */
double safe_sqrt(double value) {
    if (value < 0.0) exit(EXIT_FAILURE);
    return sqrt(value);
}


/**
 * Square the given number.
 * @param  value -> The number to be squared.
 * @return the square of the given number.
 */
double square(double value) {
    return pow(value, 2.0);
}


/**
 * Initialize the estimator.
 * @param  estimate -> The x_k value
 * @param  x_file_name -> The file containing the x-axis data.
 * @param  y_file_name -> The file containing the y-axis data.
 */
Estimator::Estimator(double estimate,
                     std::string x_file_name,
                     std::string y_file_name) {
    _xk = estimate;
    loadData(x_file_name, y_file_name);
    calculateEstimate();
}


/**
 * Run all the necessary estimation functions.
 */
void Estimator::calculateEstimate() {
    getRegressionCoefficients();
    getStandardDeviation();
    getTValue();
    getRange();
    getPredictionIntervals();
}


/**
 * Print the results of the calculations.
 */
void Estimator::printResults() {
    printf("Beta 0:         %f\n", _beta_0);
    printf("Beta 1:         %f\n", _beta_1);
    printf("\n");
    printf("Std. Deviation: %f\n", _std_dev);
    printf("Projection:     %f\n", _yk);
    printf("\n");
    printf("70%% T-Value:    %f\n", _t_seventy);
    printf("90%% T-Value:    %f\n", _t_ninety);
    printf("\n");
    printf("70%% Range:      %f\n", _range_seventy);
    printf("90%% Range:      %f\n", _range_ninety);
    printf("\n");
    printf("70%% UPI:        %f\n", _upi_seventy);
    printf("90%% UPI:        %f\n", _upi_ninety);
    printf("\n");
    printf("70%% LPI:        %f\n", _lpi_seventy);
    printf("90%% LPI:        %f\n", _lpi_ninety);
}


/**
 * Load the appropriate data from the relevant files.
 * @param x_file_name -> The name of the file containing the x-axis data.
 * @param y_file_name -> The name of the file containing the y-axis data.
 */
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


/**
 * Calculate B_0 and B_1.
 */
void Estimator::getRegressionCoefficients() {
    _x_avg = 0.0;
    _y_avg = 0.0;
    double num_sum   = 0.0;
    double denom_sum = 0.0;
    double num       = 0.0;
    double denom     = 0.0;
    for (auto pair : data) {
        num_sum   += pair.first * pair.second;
        denom_sum += square(pair.first);
        _x_avg    += pair.first;
        _y_avg    += pair.second;
    }
    _x_avg /= _n;
    _y_avg /= _n;
    num     = (num_sum - (_n * _x_avg * _y_avg));
    denom   = (denom_sum - (_n * square(_x_avg)));
    _beta_1 = num / denom;
    _beta_0 = _y_avg - (_beta_1 * _x_avg);
    _yk     = _beta_0 + (_beta_1 * _xk);
}


/**
 * Calculate the standard deviation.
 */
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


/**
 * Get the T-value from the table.
 */
void Estimator::getTValue() {
    int dof   = _n - 2;
    int index = -1;
    for (int i = 0; i < T_TABLE_SIZE; ++i) {
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
}


/**
 * Get the estimation range for y_k.
 */
void Estimator::getRange() {
    double num = square(_xk - _x_avg);
    double denom = 0.0;
    for (auto pair : data) {
        denom += square(pair.first - _x_avg);
    }
    double square_root = safe_sqrt(1 + (1 / _n) + (num / denom));
    _range_seventy = _t_seventy * _std_dev * square_root;
    _range_ninety = _t_ninety * _std_dev * square_root;
}


/**
 * Get the prediction intervals for y_k.
 */
void Estimator::getPredictionIntervals() {
    _upi_seventy = _yk + _range_seventy;
    _upi_ninety  = _yk + _range_ninety;
    _lpi_seventy = _yk - _range_seventy;
    _lpi_ninety  = _yk - _range_ninety;
}
