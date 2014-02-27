/**
 * Copyright 2014 Andrew Brinker
 */

#include <string>

#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

namespace grammar {

class Preprocessor {
 public:
  explicit Preprocessor(std::string);

 private:
  DISALLOW_COPY_AND_ASSIGN(Preprocessor);
};

}

#endif  // PREPROCESSOR_H
