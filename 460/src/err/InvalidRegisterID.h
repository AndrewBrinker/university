/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#ifndef INVALID_REGISTER_ID_H
#define INVALID_REGISTER_ID_H

#include <exception>
#include <string>
#include "./GenericError.h"

class InvalidRegisterID : public GenericError {
 public:
  explicit InvalidRegisterID(std::string source)
    : GenericError(source) {}

  const char* what() const throw() {
    return "Invalid register ID";
  }
};

#endif  // INVALID_REGISTER_ID_H
