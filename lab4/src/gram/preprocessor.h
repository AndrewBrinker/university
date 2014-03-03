/**
 * Copyright 2014 Andrew Brinker
 */

#include <string>
#include "./GrammarFile.h"

#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

namespace gram {

class Preprocessor {
 public:
  explicit Preprocessor(std::string);
  GrammarFile run();

 private:
  GrammarFile load();
  GrammarFile expand();

  std::string name;
  GrammarFile file;
  bool _is_expanded;

  DISALLOW_COPY_AND_ASSIGN(Preprocessor);
};

}  // namespace gram

#endif  // PREPROCESSOR_H
