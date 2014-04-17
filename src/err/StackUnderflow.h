/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#ifndef STACK_UNDERFLOW_H
#define STACK_UNDERFLOW_H

#include <exception>
#include <string>
#include "./GenericError.h"

class StackUnderflow : public GenericError {
 public:
  explicit StackUnderflow(std::string source)
    : GenericError(source) {}

  const char* what() const throw() {
    return "Stack underflow";
  }
};

#endif  // STACK_UNDERFLOW_H
