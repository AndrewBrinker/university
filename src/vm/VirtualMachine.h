/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include <vector>
#include <cstdint>

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&);

class VirtualMachine {
 public:
  VirtualMachine();
  VirtualMachine(uint16_t, uint16_t);

 private:
  DISALLOW_COPY_AND_ASSIGN(VirtualMachine);

  std::vector<int> r;
  std::vector<int> mem;

  uint16_t r[4], pc, ir, sr, sp, base, limit, clock;
};

#endif  // VIRTUALMACHINE_H
