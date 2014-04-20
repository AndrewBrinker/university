/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#ifndef UNRECOGNIZED_FLAG_H
#define UNRECOGNIZED_FLAG_H

#include <exception>
#include <string>
#include "./GenericError.h"

class UnrecognizedFlag : public GenericError {
 public:
  explicit UnrecognizedFlag(std::string source)
    : GenericError(source) {}

  const char* what() const throw() {
    return "Unrecognized flag";
  }
};

#endif  // UNRECOGNIZED_FLAG_H
