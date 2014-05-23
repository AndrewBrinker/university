/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include <cstdint>
#include <map>

#ifndef TLB_H
#define TLB_H

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;      \
  void operator=(const TypeName&) = delete;
#endif

class TLB {
 public:
  TLB() : data(std::map<uint8_t, uint8_t>());
  std::map<uint8_t, uint8_t> data;

 private:
  DISALLOW_COPY_AND_ASSIGN(Assembler)
};

#endif  // TLB_H
