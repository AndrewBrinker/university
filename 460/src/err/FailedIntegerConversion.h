/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#ifndef FAILED_INTEGER_CONVERSION_H
#define FAILED_INTEGER_CONVERSION_H

#include <exception>
#include <string>
#include "./GenericError.h"

class FailedIntegerConversion : public GenericError {
 public:
  explicit FailedIntegerConversion(std::string source)
    : GenericError(source) {}

  const char* what() const throw() {
    return "Failed integer conversion";
  }
};

#endif  // FAILED_INTEGER_CONVERSION_H
