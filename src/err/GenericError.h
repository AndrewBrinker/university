/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#ifndef GENERIC_ERROR_H
#define GENERIC_ERROR_H

#include <exception>
#include <string>
#include <cstdlib>

class GenericError : public std::exception {
 public:
  explicit GenericError(std::string source)
    : _source(source) {}

  std::string source() const throw() {
    return _source;
  }

  void reportError() {
    printf("%s error: %s. Terminating...\n", source().c_str(), what());
    exit(EXIT_FAILURE);
  }

 private:
  std::string _source;
};

#endif  // GENERIC_ERROR_H
