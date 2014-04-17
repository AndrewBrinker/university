/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#ifndef INVALID_INTEGER_CONVERSION_MODE_H
#define INVALID_INTEGER_CONVERSION_MODE_H

#include <exception>
#include <string>
#include "./GenericError.h"

class InvalidIntegerConversionMode : public GenericError {
 public:
  explicit InvalidIntegerConversionMode(std::string source)
    : GenericError(source) {}

  const char* what() const throw() {
    return "Invalid integer conversion mode";
  }
};

#endif  // INVALID_INTEGER_CONVERSION_MODE_H
