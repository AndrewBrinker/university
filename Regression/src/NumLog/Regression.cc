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

Regression::Regression(std::string file_name) {
    NumberLog n(file_name);
    data = n.getData();
    calculate();
}


void Regression::calculate() {
    // Do calculations
}
