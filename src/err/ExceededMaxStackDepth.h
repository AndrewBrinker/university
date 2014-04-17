/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#ifndef EXCEEDED_MAX_STACK_DEPTH_H
#define EXCEEDED_MAX_STACK_DEPTH_H

#include <exception>
#include <string>
#include "./GenericError.h"

class ExceededMaxStackDepth : public GenericError {
 public:
  explicit ExceededMaxStackDepth(std::string source)
    : GenericError(source) {}

  const char* what() const throw() {
    return "Exceeded max stack depth";
  }
};

#endif  // EXCEEDED_MAX_STACK_DEPTH_H
