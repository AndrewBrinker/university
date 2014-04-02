/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include "./VirtualMachine.h"

#define REG_FILE_SIZE 4
#define MEM_SIZE      256

VirtualMachine::VirtualMachine()
  : VirtualMachine(REG_FILE_SIZE, MEM_SIZE) {}

VirtualMachine::VirtualMachine(unsigned int reg_file_size,
                               unsigned int mem_size) :
                               r(reg_file_size),
                               mem(mem_size) {}
