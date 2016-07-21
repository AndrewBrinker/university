/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#ifndef CANT_FIT_IN_MEMORY_H
#define CANT_FIT_IN_MEMORY_H

#include <exception>
#include <string>
#include "./GenericError.h"

class CantFitInMemory : public GenericError {
 public:
  explicit CantFitInMemory(std::string source)
    : GenericError(source) {}

  const char* what() const throw() {
    return "Program can't fit in memory";
  }
};

#endif  // CANT_FIT_IN_MEMORY_H
