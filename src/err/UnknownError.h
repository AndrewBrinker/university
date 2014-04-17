/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#ifndef UNKNOWN_ERROR_H
#define UNKNOWN_ERROR_H

#include <exception>
#include <string>
#include "./GenericError.h"

class UnknownError : public GenericError {
 public:
  explicit UnknownError(std::string source)
    : GenericError(source) {}

  const char* what() const throw() {
    return "Unknown error";
  }
};

#endif  // UNKNOWN_ERROR_H
