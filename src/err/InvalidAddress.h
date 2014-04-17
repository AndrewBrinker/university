/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#ifndef INVALID_ADDRESS_H
#define INVALID_ADDRESS_H

#include <exception>
#include <string>
#include "./GenericError.h"

class InvalidAddress : public GenericError {
 public:
  explicit InvalidAddress(std::string source)
    : GenericError(source) {}

  const char* what() const throw() {
    return "Invalid address";
  }
};

#endif  // INVALID_ADDRESS_H
