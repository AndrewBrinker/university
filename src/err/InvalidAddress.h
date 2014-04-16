/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include <exception>

class InvalidAddress : public std::exception {
  const char* what() const throw() {
    return "Invalid address";
  }
};
