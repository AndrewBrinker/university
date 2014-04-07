/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include <exception>

class InvalidFileName : public std::exception {
  const char* what() const noexcept {
    return "Invalid file name.";
  }
};
