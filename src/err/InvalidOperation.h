/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include <exception>

class InvalidOperation : public std::exception {
  const char* what() const throw() {
    return "Invalid operation";
  }
};
