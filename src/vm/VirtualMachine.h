/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include <vector>

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&);

class VirtualMachine {
 public:
  VirtualMachine();
  VirtualMachine(unsigned int, unsigned int);

 private:
  DISALLOW_COPY_AND_ASSIGN(VirtualMachine);

  std::vector<int> r;
  std::vector<int> mem;

};

#endif  // VIRTUALMACHINE_H
