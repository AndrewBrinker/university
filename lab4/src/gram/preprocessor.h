/**
 * Copyright 2014 Andrew Brinker
 */

#include <string>
#include "./grammarfile.h"

#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

namespace gram {

class preprocessor {
 public:
  explicit preprocessor(std::string);
  grammarfile run();

 private:
  grammarfile load();
  grammarfile expand();

  std::string name;
  grammarfile file;
  bool _is_expanded;

  DISALLOW_COPY_AND_ASSIGN(preprocessor);
};

}  // namespace gram

#endif  // PREPROCESSOR_H
