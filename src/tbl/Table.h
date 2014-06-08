/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include <cstdint>
#include <array>

#ifndef TABLE_H
#define TABLE_H

#define N_FRAMES  32

struct TableEntry {
  TableEntry();
  explicit TableEntry(const uint8_t &);

  uint8_t frame    : 5;
  bool    modified : 1;
  bool    valid    : 1;
};

typedef std::array<TableEntry, N_FRAMES> TLB;

#endif  // TABLE_H
