/**
 * Copyright 2014 Andrew Brinker
 */

#include <set>
#include "./Item.h"

#ifndef LRSET_H
#define LRSET_H

class LRSet {
 public:
  LRSet(std::set<Item>, int, char);
  LRSet(const LRSet &);
  LRSet& operator=(const LRSet &);
  bool operator<(const LRSet&) const;

  std::set<Item> data;
  int identifier;
  char transition_code;
};

#endif  // LRSET_H
