/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include <exception>

class ImproperFilePermissions : public std::exception {
  const char* what() const noexcept {
    return "Improper file permissions.";
  }
};
