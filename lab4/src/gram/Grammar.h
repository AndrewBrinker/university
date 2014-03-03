/**
 * Copyright 2014 Andrew Brinker
 */

#include <string>
#include <list>

#ifndef GRAMMAR_H
#define GRAMMAR_H

namespace gram {

class Grammar {
 public:
  Grammar();
  explicit Grammar(std::list<std::string>);
  Grammar(const Grammar &);

  // Assignment operator
  Grammar& operator=(const Grammar &);

  // Variables
  std::list<std::string> contents;
};

}  // namespace gram

#endif  // GRAMMAR_H
