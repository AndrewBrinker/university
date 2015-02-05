/**
 * Copyright 2014 Andrew Brinker
 */

#include <string>
#include <TransitionTable.h>

#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&);


class Preprocessor {
 public:
  explicit Preprocessor();
  TransitionTable run(std::string);

  DISALLOW_COPY_AND_ASSIGN(Preprocessor);
};

#endif  // PREPROCESSOR_H
