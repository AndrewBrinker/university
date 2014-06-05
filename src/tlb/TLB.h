/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include <cstdint>
#include <array>

#ifndef TLB_H
#define TLB_H

#define PAGE_SIZE 8
#define N_FRAMES  32

typedef struct TLB_Entry {
  TLB_Entry()
    : frame(0), modified(0), valid(0), empty(0) {}
  explicit TLB_Entry(const uint8_t &f)
    : frame(f), modified(0), valid(0), empty(0) {}

  uint8_t frame    : 5;
  bool    modified : 1;
  bool    valid    : 1;
  bool    empty    : 1;
} TLB_Entry;

typedef std::array<TLB_Entry, N_FRAMES> TLB;

#endif  // TLB_H
