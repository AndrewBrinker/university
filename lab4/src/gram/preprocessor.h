/**
 * Copyright 2014 Andrew Brinker
 */

#include <string>
#include "./Grammar.h"

#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&);

namespace gram {

class Preprocessor {
 public:
  explicit Preprocessor(std::string);
  Grammar run();

 private:
  Grammar load();
  Grammar expand();

  std::string name;
  Grammar file;
  bool _is_expanded;

  DISALLOW_COPY_AND_ASSIGN(Preprocessor);
};

}  // namespace gram

#endif  // PREPROCESSOR_H
