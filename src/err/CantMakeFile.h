/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#ifndef CANT_MAKE_FILE_H
#define CANT_MAKE_FILE_H

#include <exception>
#include <string>
#include "./GenericError.h"

class CantMakeFile : public GenericError {
 public:
  explicit CantMakeFile(std::string source)
    : GenericError(source) {}

  const char* what() const throw() {
    return "The file cannot be made";
  }
};

#endif  // CANT_MAKE_FILE_H
