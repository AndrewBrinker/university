/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#ifndef INVALID_FILE_NAME_H
#define INVALID_FILE_NAME_H

#include <exception>
#include <string>
#include "./GenericError.h"

class InvalidFileName : public GenericError {
 public:
  explicit InvalidFileName(std::string source)
    : GenericError(source) {}

  const char* what() const throw() {
    return "Invalid file name";
  }
};

#endif  // INVALID_FILE_NAME_H
