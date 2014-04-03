/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include "../asm/Assembler.h"
#include <vector>
#include <list>
#include <cstdint>

#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&);

class VirtualMachine {
 public:
  VirtualMachine();
  VirtualMachine(uint16_t, uint16_t);
  void run(Assembler::ObjectSource);

 private:
  DISALLOW_COPY_AND_ASSIGN(VirtualMachine);

  std::vector<int> r;
  std::vector<int> mem;
};

#endif  // VIRTUALMACHINE_H