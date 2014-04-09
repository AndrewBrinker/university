/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include <exception>

class CantMakeFile : public std::exception {
  const char* what() const noexcept {
    return "The file cannot be made";
  }
};
