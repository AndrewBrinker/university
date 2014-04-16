/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include <exception>

class CantOpenFile : public std::exception {
  const char* what() const throw() {
    return "Can't open file";
  }
};
