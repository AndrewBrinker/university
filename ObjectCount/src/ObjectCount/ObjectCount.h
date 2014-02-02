/**
 * Name:       Andrew Brinker
 * Class:      CSE 455, Software Engineering (Winter 2014)
 * School:     California State University, San Bernardino
 * Assignment: 2A
 *
 * Copyright 2014 Andrew Brinker
 */

#include <string>
#include <istream>
#include "./LineCount.h"

#ifndef LC_OBJECT_COUNT_H
#define LC_OBJECT_COUNT_H

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&);

namespace lc {

class ObjectCount : public LineCount {
 public:
  ObjectCount();
  void operator<<(std::string);
  void scan(std::string);

  void clear();
  unsigned methods();

 private:
  void removeExtension(std::string*);

  unsigned _method_count;

  DISALLOW_COPY_AND_ASSIGN(ObjectCount);
};

static ObjectCount count;

}  // End namespace lc

#endif  // LC_OBJECT_COUNT_H
