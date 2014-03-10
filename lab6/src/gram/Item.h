/**
 * Copyright 2014 Andrew Brinker
 */

#include <string>

#ifndef ITEM_H
#define ITEM_H

class Item {
 public:
  Item(std::string, int);
  Item(const Item &);
  Item& operator=(const Item &);
  bool operator<(const Item&) const;
  std::string production;
  int dot;
};

#endif  // ITEM_H
