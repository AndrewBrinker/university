/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include "./VirtualMachine.h"
#include <cstdint>

#define REG_FILE_SIZE 4
#define MEM_SIZE      256

VirtualMachine::VirtualMachine()
  : VirtualMachine(REG_FILE_SIZE, MEM_SIZE) {}


VirtualMachine::VirtualMachine(uint16_t reg_file_size,
                               uint16_t mem_size) :
                               mem(mem_size) {}


void run(std::list<uint16_t>) {
    return;
}
