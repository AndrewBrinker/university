/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include <cstdint>
#include <vector>

#ifndef TLB_H
#define TLB_H

struct TLB_Entry {
  TLB_Entry();
  TLB_Entry( const uint8_t& );

  uint8_t frame    : 5;
  bool    modified : 1;
  bool    valid    : 1;
  bool    empty    : 1;
};

typedef std::vector< TLB_Entry > TLB;

#endif  // TLB_H
