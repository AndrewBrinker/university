/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#ifndef CANT_OPEN_FILE_H
#define CANT_OPEN_FILE_H

#include <exception>
#include <string>
#include "./GenericError.h"

class CantOpenFile : public GenericError {
 public:
  explicit CantOpenFile(std::string source)
    : GenericError(source) {}

  const char* what() const throw() {
    return "The file cannot be opened";
  }
};

#endif  // CANT_OPEN_FILE_H
