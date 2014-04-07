/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include <exception>

class FileDoesNotExist : public std::exception {
  const char* what() const noexcept {
    return "File does not exist.";
  }
};
