/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include <exception>

class FailedIntegerConversion : public std::exception {
  const char* what() const noexcept {
    return "Failed integer conversion";
  }
};
