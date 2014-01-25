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

#ifndef LC_LINECOUNT_H
#define LC_LINECOUNT_H

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

namespace lc {

class LineCount {
 public:
  LineCount();
  void operator<<(std::string);
  void scan(std::string);
  void clear();
  bool good();
  bool bad();
  unsigned loc();

 private:
  bool fileExists(std::string);

  bool _is_good;
  unsigned _loc;

  DISALLOW_COPY_AND_ASSIGN(LineCount);
};

static LineCount count;

}  // End namespace lc

#endif  // LC_LINECOUNT_H
