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
  union tlb_entry_t {
    uint8_t i;
    struct {
      uint8_t frame  : 5;
      uint8_t modify : 1;
      uint8_t valid  : 1;
    } fmt0;
  }

  TLB() : data(std::map<tlb_entry_t, uint8_t>());
  std::map<tlb_entry_t, uint8_t> data;

 private:
  DISALLOW_COPY_AND_ASSIGN(Assembler)
};

#endif  // TLB_H
