/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include <cstdint>
#include <array>

#ifndef TLB_H
#define TLB_H

#define PAGE_SIZE 8
#define N_FRAMES  32

struct TLB_Entry {
  TLB_Entry();
  explicit TLB_Entry(const uint8_t &);

  uint8_t frame    : 5;
  bool    modified : 1;
  bool    valid    : 1;
  bool    empty    : 1;
};

typedef std::array<TLB_Entry, N_FRAMES> TLB;

#endif  // TLB_H
