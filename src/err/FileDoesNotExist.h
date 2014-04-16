/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include <exception>

class FileDoesNotExist : public std::exception {
  const char* what() const throw() {
    return "File does not exist";
  }
};
