/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include <exception>

class InvalidRegisterID : public std::exception {
  const char* what() const throw() {
    return "Invalid register ID";
  }
};
