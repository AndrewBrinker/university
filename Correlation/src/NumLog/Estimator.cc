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
#include <numeric>
#include <cstdlib>
#include "./NumberLog.h"


/**
 * Exit with a short exit message.
 * @param code -> The exit code, passed directly to exit()
 * @param msg  -> The message to be printed for the user.
 */
void exit_with_error(int code, std::string msg) {
    printf("%s", msg.c_str());
    exit(code);
}


/**
 * Get the square root of a number, and quit loudly if the number is negative.
 * @param  value -> The value being square-rooted.
 * @return the square root of the value.
 */
double safe_sqrt(double value) {
    if (value < 0.0) {
        exit_with_error(EXIT_FAILURE,
                        "Can't square root a negative number. Exiting...\n");
    }
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
Estimator::Estimator(std::string x_file_name,
                     std::string y_file_name) {
    loadData(x_file_name, y_file_name);
    calculateEstimate();
}


/**
 * Run all the necessary estimation functions.
 */
void Estimator::calculateEstimate() {
    getCorrelation();
    getSignificance();
}


/**
 * Print the results of the calculations.
 */
void Estimator::printResults() {
    printf("Correlation:    %f\n", _r);
    printf("Significance:   %f\n", _s);
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
 * Get the correlation coefficient of the two data sets.
 */
void Estimator::getCorrelation() {
    double pair_sum     = 0.0;
    double x_sum        = 0.0;
    double y_sum        = 0.0;
    double x_square_sum = 0.0;
    double y_square_sum = 0.0;
    double num          = 0.0;
    double denom        = 0.0;
    double denom_x      = 0.0;
    double denom_y      = 0.0;
    for (auto pair : data) {
        pair_sum     += pair.first * pair.second;
        x_sum        += pair.first;
        y_sum        += pair.second;
        x_square_sum += square(pair.first);
        y_square_sum += square(pair.second);
    }
    num     = (_n * pair_sum) - (x_sum * y_sum);
    denom_x = ((_n * x_square_sum) - square(x_sum));
    denom_y = ((_n * y_square_sum) - square(y_sum));
    denom   = safe_sqrt(denom_x * denom_y);
    _r      = num / denom;
}


/**
 * Get the significance of the given correlation coefficient
 */
void Estimator::getSignificance() {
    _s = (_r) / (safe_sqrt((1.0 - square(_r)) / ((_n - 2))));
}
