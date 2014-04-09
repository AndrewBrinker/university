/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include <exception>

class InvalidFileName : public std::exception {
 public:
  const char *what() const throw() {
    return "Unknown error";
  }
};
