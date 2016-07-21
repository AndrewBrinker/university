/**
 * Copyright 2014 Andrew Brinker
 */

#include <string>
#include <list>

#ifndef GRAMMAR_H
#define GRAMMAR_H

class Grammar {
 public:
  Grammar();
  explicit Grammar(std::list<std::string>);
  Grammar(const Grammar &);
  Grammar& operator=(const Grammar &);

  std::list<std::string> contents;
};

#endif  // GRAMMAR_H
