/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#ifndef INVALID_OPERATION_H
#define INVALID_OPERATION_H

#include <exception>
#include <string>
#include "./GenericError.h"

class InvalidOperation : public GenericError {
 public:
  explicit InvalidOperation(std::string source)
    : GenericError(source) {}

  const char* what() const throw() {
    return "Invalid operation";
  }
};

#endif  // INVALID_OPERATION_H
