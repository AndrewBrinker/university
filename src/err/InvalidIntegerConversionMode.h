/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include <exception>

class InvalidIntegerConversionMode : public std::exception {
 public:
  const char *what() const throw() {
    return "Invalid integer conversion mode";
  }
};
