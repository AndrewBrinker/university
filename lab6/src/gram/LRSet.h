/**
 * Copyright 2014 Andrew Brinker
 */

#include <string>
#include <set>

#ifndef LRSET_H
#define LRSET_H

class LRSet {
 public:
  LRSet(std::set<std::string>, int, char);
  LRSet(const LRSet &);
  LRSet& operator=(const LRSet &);

  std::set<std::string> data;
  int identifier;
  char transition_code;
};

#endif  // LRSET_H
