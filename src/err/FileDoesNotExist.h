/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#ifndef FILE_DOES_NOT_EXIST_H
#define FILE_DOES_NOT_EXIST_H

#include <exception>
#include <string>
#include "./GenericError.h"

class FileDoesNotExist : public GenericError {
 public:
  explicit FileDoesNotExist(std::string source)
    : GenericError(source) {}

  const char* what() const throw() {
    return "Files does not exist.";
  }
};

#endif  // FILE_DOES_NOT_EXIST_H
