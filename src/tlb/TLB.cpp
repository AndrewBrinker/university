/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include "./TLB.h"

TLB_Entry::TLB_Entry()
  : frame(0),
    modified(0),
    valid(0),
    __unused(0) {}

TLB_Entry::TLB_Entry(const uint8_t& i)
  : frame(i),
    modified(0),
    valid(1),
    __unused(0) {}
