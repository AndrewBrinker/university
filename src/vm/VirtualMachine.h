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
  VirtualMachine() : VirtualMachine(REG_FILE_SIZE, MEM_SIZE) {}

  VirtualMachine(unsigned int reg_file_size, unsigned int mem_size) :
    r(reg_file_size),
    mem(mem_size) {}

 private:
  DISALLOW_COPY_AND_ASSIGN(VirtualMachine);

  static const int REG_FILE_SIZE = 4;
  static const int MEM_SIZE = 256;

  std::vector<int> r;
  std::vector<int> mem;

};

#endif  // VIRTUALMACHINE_H
