/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include "./Table.h"

TableEntry::TableEntry()
  : frame(0),
    modified(0),
    valid(0) {}

TableEntry::TableEntry(const uint8_t& i)
  : frame(i),
    modified(0),
    valid(1) {}
